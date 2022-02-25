#include "FrameEncoderBank.h"
#include "CwipcFrameEncoder.h"
#include "ddk_reference_wrapper.h"
#include "FusionModuleProfiler.h"
#include "MCUManagerDefs.h"

namespace mcu
{

FrameEncoderBank::FrameEncoderBank()
: m_maxPendingDecodedFrames(0)
{
	pthread_mutex_init(&m_mutex,nullptr);
}
FrameEncoderBank::~FrameEncoderBank()
{
	std::map<EncodecType,ifusioned_frame_encoder_unique_ref>::iterator itEncoder = m_encoders.begin();
	for (;itEncoder!=m_encoders.end();++itEncoder)
	{
		itEncoder->second->stop();
	}

	pthread_mutex_destroy(&m_mutex);

	if(m_taskExecutor)
	{
		m_taskExecutor->stop();
	}
}
void FrameEncoderBank::init(const encoder_context& i_context)
{
	pfr::iprofiler_lent_ref mcuProfiler = pfr::IProfiler::get_instance();

	m_maxPendingDecodedFrames = i_context.m_maxPendingDecodedFrames;

	m_taskExecutor = ddk::make_unique_reference<ddk::task_executor>(i_context.m_numThreadsPerEncoder,0);

	m_taskExecutor->start();

	m_measuremntConn.push_back(mcuProfiler->listen_to<SentFrameRateMeasurable>(k_fusion_profile_module, ddk::make_function(this,&FrameEncoderBank::checkEncoderFrameRate)));
}
FrameEncoderBank::register_result FrameEncoderBank::register_frame_dest(frame_dest_id i_id, EncodecType i_codecType, scene_builder_lent_ref i_collector)
{
	ifusioned_frame_encoder_lent_ptr currEncoder = nullptr;

	pthread_mutex_lock(&m_mutex);

	std::map<frame_dest_id,scene_builder_lent_ref>::iterator itBuilder = m_sceneBuilders.find(i_id);
	if(itBuilder == m_sceneBuilders.end())
	{
		m_sceneBuilders.insert(std::make_pair(i_id,i_collector));

		//first check if we have encoder for this type
		std::map<EncodecType,ifusioned_frame_encoder_unique_ref>::iterator itEncoder = m_encoders.find(i_codecType);
		if (itEncoder == m_encoders.end())
		{
			try
			{
				if(m_taskExecutor)
				{
					ifusioned_frame_encoder_unique_ref newEncoder = m_encoderFactory.create_frame_encoder(i_codecType,i_id,ddk::promote_to_ref(ddk::lend(m_taskExecutor)),m_maxPendingDecodedFrames);

					newEncoder->start();

					currEncoder = ddk::lend(newEncoder);

					m_encoders.insert(std::make_pair(i_codecType,ddk::promote_to_ref(std::move(newEncoder))));
				}
				else
				{
					return ddk::make_error<register_result>(RegisterErrorCode::RegisterLackOfResources);
				}
			}
			catch (const FusionEncoderException&)
			{
				pthread_mutex_unlock(&m_mutex);

				return ddk::make_error<register_result>(RegisterErrorCode::RegisterInternalError);
			}
		}
		else
		{
			currEncoder = ddk::lend(itEncoder->second);
		}
	}
	else
	{
		return ddk::make_error<register_result>(RegisterErrorCode::RegisterCollectorAlreadyExist);
	}

	m_taskExecutor->set_max_num_pending_tasks(m_taskExecutor->get_max_num_pending_tasks() + m_maxPendingDecodedFrames);

	pthread_mutex_unlock(&m_mutex);

	i_collector->call_onFrameToEncode = ddk::make_function(ddk::get_raw_ptr(currEncoder),&IFusionedFrameEncoder::encode_frame);

	return ddk::success;
}
FrameEncoderBank::unregister_result FrameEncoderBank::unregister_frame_dest(frame_dest_id i_id)
{
	pthread_mutex_lock(&m_mutex);

	std::map<frame_dest_id,scene_builder_lent_ref>::iterator itBuilder = m_sceneBuilders.find(i_id);
	bool somethingChanged = itBuilder != m_sceneBuilders.end();
	if(somethingChanged)
	{
		m_taskExecutor->set_max_num_pending_tasks(m_taskExecutor->get_max_num_pending_tasks() - m_maxPendingDecodedFrames);

		m_sceneBuilders.erase(itBuilder);
	}

	pthread_mutex_unlock(&m_mutex);

	if(somethingChanged)
	{
		sig_onStateChanged.execute();
	}

	return ddk::success;
}
void FrameEncoderBank::notify_resource_change()
{
	sig_onStateChanged.execute();
}
size_t FrameEncoderBank::needed_resources() const
{
	return k_numCpusForEncoding;

	pthread_mutex_lock(&m_mutex);

	const size_t k_numBuilders = m_sceneBuilders.size();

	float res = 0.f;

	for(const auto& builder : m_sceneBuilders)
	{
		//this computation is attached to a certain amount of points per pointcloud (still not applying any lod). Right now is 30k
		res += (50.75f * builder.second->size() - 16.f);
	}

	pthread_mutex_unlock(&m_mutex);

	return static_cast<size_t>(res / 66.f);
}
void FrameEncoderBank::set_affinity(const cpu_set_t& i_cpuSet)
{
	size_t res = m_taskExecutor->set_affinity(i_cpuSet);
	int a = 0;
}
void FrameEncoderBank::checkEncoderFrameRate(const frame_sent_fps& i_measure)
{
	//typedef typename std::map<frame_dest_id, ifusioned_frame_encoder_unique_ref>::iterator iterator;

	//pthread_mutex_lock(&m_mutex);

	//iterator itEncoder = m_encoders.find(i_measure.get_id());

	//if (itEncoder != m_encoders.end())
	//{
	//	//think about setting this value into a config file
	//	if (i_measure.get_fps() < 25.f && itEncoder->second->num_threads() < m_maxThreadsPerEncoder)
	//	{
	//		ddk::thread_pool::acquire_result<ddk::thread> acqRes = m_threadPool.aquire_thread();
	//		if (acqRes.hasError() == false)
	//		{
	//			itEncoder->second->lend_thread(acqRes.extractPayload());
	//		}
	//	}
	//}
	//pthread_mutex_unlock(&m_mutex);
}

}