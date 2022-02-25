#include "MCUFusionManager.h"
#include "UniversalFusionCollection.h"
#include "PerPlayerFusionCollection.h"
#include "IMCUProfiler.h"
#include "OperationsRateMeasurable.h"
#include "FusionModuleProfiler.h"
#include "AcquisitionModuleProfiler.h"
#include "ResourceManager.h"

namespace mcu
{

MCUFusionManager::MCUFusionManager(const McuConfigProvider& i_configProvider, player_provider_const_lent_ref i_playerProvider)
: m_playerProvider(i_playerProvider)
, m_mcuConfig(i_configProvider.getMCUConfig())
, m_frameWriterConfig(i_configProvider.getFrameWriterConfigProvider().getConfig())
, m_fusionCtr(m_mcuConfig.get_max_num_decoded_pending_frames())
{
}
MCUFusionManager::~MCUFusionManager()
{
}
void MCUFusionManager::start()
{
	try
	{
		pfr::iprofiler_lent_ref mcuProfiler = pfr::IProfiler::get_instance();

		//ddk::unique_reference_wrapper<UniversalFusionCollection> fusioner = ddk::make_unique_reference<UniversalFusionCollection>(m_fusionCtr,m_mcuConfig);
		ddk::unique_reference_wrapper<PerPlayerFusionCollection> fusioner = ddk::make_unique_reference<PerPlayerFusionCollection>(m_fusionCtr,m_mcuConfig);

		ResourceManager::get_instance()->resgiter_resource_consumer(ddk::promote_to_ref(ddk::lend(fusioner)),true);

		const encoder_context regContext{ EncodecType::Cwipc, FrameWriterType::SockettIo, m_frameWriterConfig.getExchangeName(), m_frameWriterConfig.getUrl(), m_mcuConfig.get_num_encoder_trheads(), m_mcuConfig.get_max_num_decoded_pending_frames() };
		fusioner->init(regContext);

		m_fusionCollection = std::move(fusioner);

		//mcuProfiler->listen_to<SentFrameRateMeasurable>(k_fusion_profile_module,[=](frame_sent_fps i_measure)
		//{
		//	const float currFps = i_measure.get_fps();
		//	PlayerLod currLod = PlayerLod::Lod0;

		//	if(currFps < 25.f && currFps > 20.f)
		//	{
		//		currLod = PlayerLod::Lod1;
		//	}
		//	else if(currFps < 20.f && currFps > 10.f)
		//	{
		//		currLod = PlayerLod::Lod2;
		//	}
		//	else if(currFps < 10.f)
		//	{
		//		currLod = PlayerLod::Lod3;
		//	}

		//	m_fusionCollection->set_global_lod(currLod);
		//});
	}
	catch (const FusionEncoderException&)
	{
		DDK_LOG_ERROR("Fusion collector could not be started: invalid frame encoder");
	}
	catch (const FusionSenderException&)
	{
		DDK_LOG_ERROR("Fusion collector could not be started: invalid frame sender");
	}
}
MCUFusionManager::register_result MCUFusionManager::register_frame_source_id(frame_source_id i_id, iphysical_object_const_lent_ref i_obj, const encoder_context& i_context)
{
	try
	{
		if (m_fusionCtr.register_frame_source(i_id,i_obj))
		{
			m_fusionCollection->add_id(i_id,i_obj,i_context);

			return ddk::success;
		}
		else
		{
			return ddk::make_error<register_result>(RegisterErrorCode::SourceAlreadyRegistered);
		}
	}
	catch (const FusionCreationException&)
	{
		return ddk::make_error<register_result>(RegisterErrorCode::SourceAlreadyRegistered);
	}
	catch (const FusionSenderException&)
	{
		return ddk::make_error<register_result>(RegisterErrorCode::InvalidFrameWriterFormat);
	}
	catch (const FusionEncoderException&)
	{
		return ddk::make_error<register_result>(RegisterErrorCode::InvalidFrameEncoderFormat);
	}
}
MCUFusionManager::unregister_result MCUFusionManager::unregister_frame_source_id(frame_source_id i_id)
{
	m_fusionCollection->remove_id(i_id);

	return (m_fusionCtr.unregister_frame_source(i_id)) ? ddk::success : ddk::make_error<unregister_result>(UnregisterErrorCode::SourceNotRegistered);
}
void MCUFusionManager::enqueueDecodedFrame(frame_source_id i_sourceId, DecodedFrame i_frame)
{
	m_fusionCtr.add_decoded_frame(i_sourceId,std::move(i_frame));
}

}