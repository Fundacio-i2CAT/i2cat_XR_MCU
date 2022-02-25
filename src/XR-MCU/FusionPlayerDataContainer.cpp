#include "FusionPlayerDataContainer.h"
#include "DecodedFrameTransformVisitor.h"
#include "AcquisitionModuleProfiler.h"
#include "IMCUProfiler.h"
#include "AcquisitionModuleProfiler.h"

namespace mcu
{

FusionPlayerDataContainer::FusionPlayerDataContainer(frame_source_id i_id, size_t i_maxNumPendingFrames,iphysical_object_const_lent_ref i_object, const IFusionNotifier& i_notifier)
: m_id(i_id)
, m_maxNumPendingFrames(i_maxNumPendingFrames)
, m_object(i_object)
, m_notifier(i_notifier)
, m_pendingFrames(0)
, m_transformedContainer(3)
, m_numTransformedFrames(0)
{
	m_transformerExecutor.start_thread(ddk::make_function(this,&FusionPlayerDataContainer::checkForNewDecodedFrames),[this](){ return m_pendingFramesToBeTransformed.empty() == false; });

	START_MEASURE(k_acquisition_profile_module,pfr::make_measurable<TransformedFrameRateMeasurable>(ddk::make_function(this,&FusionPlayerDataContainer::checkTransformedFrameRate),std::chrono::seconds(1)),m_transformRateMeasureId);
}
void FusionPlayerDataContainer::add_decoded_frame(DecodedFrame i_frame)
{
	if (m_pendingFrames.get() > m_maxNumPendingFrames)
	{
		m_pendingFramesToBeTransformed.pop();
	}
	else
	{
		ddk::atomic_post_increment(m_pendingFrames);
	}

	m_pendingFramesToBeTransformed.push(std::move(i_frame));

	m_transformerExecutor.signal_thread();
}
bounding_box FusionPlayerDataContainer::get_bounding_box() const
{
	return (m_transformedContainer.empty() == false) ? m_transformedContainer.last()->second.get_bounding_box() : bounding_box();
}
FusionPlayerDataContainer::const_iterator FusionPlayerDataContainer::last() const
{
	return m_transformedContainer.last();
}
bool FusionPlayerDataContainer::empty() const
{
	return m_transformedContainer.empty();
}
void FusionPlayerDataContainer::checkForNewDecodedFrames()
{
	while (ddk::optional<DecodedFrame> newDecodedFrameOpt = m_pendingFramesToBeTransformed.pop())
	{
		ddk::atomic_post_decrement(m_pendingFrames);

		DecodedFrame newDecodedFrame = std::move(*newDecodedFrameOpt);

		if(newDecodedFrame.empty() == false)
		{
			//transform frame according to current player transformation
			const pfr::profiler_time_point beforeOp = pfr::profiler_clock::now();

			try
			{
				DecodedFrameTransformVisitor transformer(m_object->get_transformation());

				newDecodedFrame.visit(transformer);

				PUBLISH_MEASURE(k_acquisition_profile_module,TransformTimeMeasurable,std::make_pair(m_id,std::chrono::seconds(1)),m_id,beforeOp)

				m_transformedContainer.add_frame(newDecodedFrame.get_time_stamp(),std::move(newDecodedFrame));

				ddk::atomic_post_increment(m_numTransformedFrames);

				m_notifier.onNewAvailableFrame();
			}
			catch(const std::exception& i_excp)
			{
				DDK_LOG_ERROR("Error transforming frame: " << i_excp.what());
			}
		}
	}
}
frame_decoder_fps FusionPlayerDataContainer::checkTransformedFrameRate(const pfr::profiler_time_point& i_time)
{
	const float fps = (m_lastTransfomedUpdateTime != pfr::profiler_time_point{}) ? static_cast<float>(m_numTransformedFrames.get()) / std::chrono::duration_cast<std::chrono::milliseconds>(i_time - m_lastTransfomedUpdateTime).count() : m_numTransformedFrames.get();

	m_numTransformedFrames.set(0);
	m_lastTransfomedUpdateTime = i_time;

	return { m_id, fps * 1000 };
}

}