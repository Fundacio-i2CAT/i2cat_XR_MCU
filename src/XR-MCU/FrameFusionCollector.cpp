#include "FrameFusionCollector.h"
#include "IMCUProfiler.h"
#include "FusionModuleProfiler.h"
#include "OperationsRateMeasurable.h"
#include "DecodedFrame.h"
#include "DecodedFrameCreateVisitor.h"
#include "MCUManagerDefs.h"
#include "ddk_async.h"

namespace mcu
{

FrameFusionCollector::FrameFusionCollector(frame_dest_id i_id, iphysical_object_const_lent_ptr i_obj, const IFusionProvider& i_fusionProvider, frame_writer_unique_ref i_frameWriter, const McuConfig& i_config)
: m_id(i_id)
, m_physicalObject(i_obj)
, m_fusionProvider(i_fusionProvider)
, m_frameWriter(std::move(i_frameWriter))
, m_numFusionedFrames(0)
, m_numSentFrames(0)
, m_numEncodedFramesToSent(i_config.get_num_encoded_frames_to_sent())
, m_fusionRateMeasureId(pfr::k_invalid_measure_id)
, m_sentRateMeasureId(pfr::k_invalid_measure_id)
, m_sentStarted(false)
, m_encodedFrames(i_config.get_max_num_decoded_pending_frames())
{
	m_updateTime = std::chrono::milliseconds(1000 / i_config.get_frame_rate());

	m_fusionExecutor.set_update_time(m_updateTime);
	m_senderExecutor.set_update_time(m_updateTime);

	pthread_mutex_init(&m_mutex,nullptr);
	pthread_mutex_init(&m_idListMutex,nullptr);

	START_MEASURE(k_fusion_profile_module, pfr::make_measurable<FusionedFrameRateMeasurable>(ddk::make_function(this,&FrameFusionCollector::checkFusionedFrameRate), std::chrono::seconds(1)), m_fusionRateMeasureId)
	START_MEASURE(k_fusion_profile_module, pfr::make_measurable<SentFrameRateMeasurable>(ddk::make_function(this,&FrameFusionCollector::checkSentFrameRate), std::chrono::seconds(1)), m_sentRateMeasureId)
}
FrameFusionCollector::FrameFusionCollector(FrameFusionCollector&& other)
: m_id(std::move(other.m_id))
, m_physicalObject(std::move(other.m_physicalObject))
, m_fusionProvider(other.m_fusionProvider)
, m_frameWriter(std::move(other.m_frameWriter))
, m_fusionExecutor(std::move(other.m_fusionExecutor))
, m_senderExecutor(std::move(other.m_senderExecutor))
, m_updateTime(other.m_updateTime)
, m_numFusionedFrames(other.m_numFusionedFrames)
, m_numSentFrames(other.m_numSentFrames)
, m_sentRateMeasureId(other.m_sentRateMeasureId)
, m_currEncodingTime(other.m_currEncodingTime)
, m_numEncodedFramesToSent(other.m_numEncodedFramesToSent)
, m_sentStarted(other.m_sentStarted)
, m_encodedFrames(std::move(other.m_encodedFrames))
{
	pthread_mutex_init(&m_mutex,nullptr);
	pthread_mutex_init(&m_idListMutex,nullptr);

	START_MEASURE(k_fusion_profile_module, pfr::make_measurable<FusionedFrameRateMeasurable>(ddk::make_function(this,&FrameFusionCollector::checkFusionedFrameRate), std::chrono::seconds(1)), m_fusionRateMeasureId)
	START_MEASURE(k_fusion_profile_module, pfr::make_measurable<SentFrameRateMeasurable>(ddk::make_function(this,&FrameFusionCollector::checkSentFrameRate), std::chrono::seconds(1)), m_sentRateMeasureId)
}
FrameFusionCollector::~FrameFusionCollector()
{
	STOP_MEASURE(k_fusion_profile_module,m_fusionRateMeasureId);
	STOP_MEASURE(k_fusion_profile_module,m_sentRateMeasureId);

	pthread_mutex_destroy(&m_idListMutex);
	pthread_mutex_destroy(&m_mutex);
}
void FrameFusionCollector::start()
{
	m_fusionExecutor.start_thread(ddk::make_function(this,&FrameFusionCollector::checkForNewDecodedFrames));
	m_senderExecutor.start_thread(ddk::make_function(this,&FrameFusionCollector::checkForNewEncodedFrames));
}
void FrameFusionCollector::stop()
{
	m_senderExecutor.stop_thread();
	m_fusionExecutor.stop_thread();
}
void FrameFusionCollector::set_affinity(const cpu_set_t& i_cpuSet)
{
	m_fusionExecutor.set_affinity(i_cpuSet);
	m_senderExecutor.set_affinity(i_cpuSet);
}
bool FrameFusionCollector::add_source(frame_source_id i_id)
{
	bool somethingChanged = false;

	pthread_mutex_lock(&m_idListMutex);

	frame_source_list_id::iterator itLod = m_ids.find(i_id);

	if (itLod == m_ids.end())
	{
		m_ids.insert(i_id);

		DDK_LOG_INFO("Adding player " << i_id << " to " << m_id);

		somethingChanged = true;
	}

	pthread_mutex_unlock(&m_idListMutex);

	return somethingChanged;
}
bool FrameFusionCollector::remove_source(frame_source_id i_id)
{
	bool somethingChanged = false;

	pthread_mutex_lock(&m_idListMutex);

	frame_source_list_id::iterator itLod = m_ids.find(i_id);

	if (itLod != m_ids.end())
	{
		DDK_LOG_INFO("Removing player " << i_id << " from " << m_id);

		m_ids.erase(itLod);

		somethingChanged = true;
	}

	pthread_mutex_unlock(&m_idListMutex);

	return somethingChanged;
}
size_t FrameFusionCollector::size() const
{
	return m_ids.size();
}
bool FrameFusionCollector::empty() const
{
	return m_ids.empty();
}
void FrameFusionCollector::onNewEncodedFrameFuture(ddk::shared_future<encoded_frame_dist_ref> i_frame)
{
	if(i_frame)
	{
		i_frame.then(ddk::make_function([this](const encoded_frame_dist_ref& ii_frame)
		{
			pthread_mutex_lock(&m_mutex);

			m_encodedFrames.add_frame(mcu_frame_clock::now(), ii_frame);

			pthread_mutex_unlock(&m_mutex);

			ddk::atomic_post_increment(m_numFusionedFrames);

			PUBLISH_MEASURE(k_fusion_profile_module,EncodingTimeMeasurable,std::make_pair(m_id,std::chrono::seconds(1)),m_id,ii_frame->get_time_stamp());

			return true;
		}));
	}
}
const transformation& FrameFusionCollector::get_transform() const
{
	if (m_physicalObject)
	{
		return m_physicalObject->get_transformation();
	}
	else
	{
		throw FusionCollectorException{};
	}
}
Frustrum FrameFusionCollector::get_frustrum() const
{
	if (m_physicalObject)
	{
		return m_physicalObject->get_frustrum();
	}
	else
	{
		throw FusionCollectorException{};
	}
}
void FrameFusionCollector::deallocate(const void* i_ptr) const
{
	if(detail::IFusionedFrameImpl* ptr = const_cast<detail::IFusionedFrameImpl*>(static_cast<const detail::IFusionedFrameImpl*>(i_ptr)))
	{
		delete ptr;
	}
	//m_availableFrames.push(m_id,const_cast<detail::IFusionedFrameImpl*>(static_cast<const detail::IFusionedFrameImpl*>(i_ptr)),*this);
}
void FrameFusionCollector::checkForNewDecodedFrames()
{
	//create a fusion
	pthread_mutex_lock(&m_idListMutex);

	IFusionProvider::scene_info sceneInfo = m_fusionProvider.get_current_scene_info(m_ids);

	pthread_mutex_unlock(&m_idListMutex);

	if (sceneInfo.empty() == false)
	{
		mcu_frame_time_point newestFrameTimePoint;
		IFusionProvider::scene_info::const_iterator itInfo = sceneInfo.begin();
		for(;itInfo!=sceneInfo.end();++itInfo)
		{
			newestFrameTimePoint = std::max(newestFrameTimePoint,itInfo->second);
		}

		//if(newestFrameTimePoint > m_currDecodingTime)
		{
			m_currDecodingTime = newestFrameTimePoint;

			try
			{
				onNewEncodedFrameFuture(ddk::eval(call_onFrameToEncode,ddk::make_function(this,&FrameFusionCollector::performFusion),sceneInfo));
			}
			catch(const std::exception& i_excp)
			{
				DDK_LOG_ERROR("Exception enqueuing new encoding task: " << i_excp.what());
			}
		}
	}
}
FusionedFrame FrameFusionCollector::performFusion(const frame_souce_time_stamp_list& i_scene)
{
	static pfr::profiler_time_point refTime = pfr::profiler_clock::now();
	const mcu_frame_time_point nowMcu = mcu_frame_clock::now();
	const pfr::profiler_time_point nowOp = pfr::profiler_clock::now();

	IFusionProvider::const_critical_section fusionCS = m_fusionProvider.enterCriticalSection<ddk::IAccessProvider::READING>(ddk::Reentrancy::NON_REENTRANT);

	IFusionProvider::frame_collection frameCollection = fusionCS->get_scene(i_scene);

	if(frameCollection.empty() == false)
	{
		IFusionProvider::frame_collection_iterator itFrameColl = frameCollection.begin();

		FusionedFrameCreateVisitor createVisitor(nowMcu);

		itFrameColl->second->second.visit(createVisitor);

		FusionedFrame fusionFrame = FusionedFrame(m_id,createVisitor.extract_impl(),*this);

		bool everyMergeOk = true;
		for(; itFrameColl != frameCollection.end() && everyMergeOk; ++itFrameColl)
		{
			try
			{
				everyMergeOk = fusionFrame.merge(itFrameColl->first,itFrameColl->second->second);
			}
			catch(const std::exception& i_excp)
			{
				everyMergeOk = false;
				DDK_LOG_ERROR("Exception merging frame: " << i_excp.what());
			}
		}

		m_fusionProvider.leaveCriticalSection<ddk::IAccessProvider::READING>(std::move(fusionCS));

		if(everyMergeOk)
		{
			for(const auto& transformedFrame : frameCollection)
			{
				PUBLISH_MEASURE(k_fusion_profile_module,FusionedFrameLatencyMeasurable,std::make_pair(transformedFrame.first,std::chrono::seconds(1)),transformedFrame.first,transformedFrame.second->second.get_time_stamp());
			}

			PUBLISH_MEASURE(k_fusion_profile_module,FusionTimeMeasurable,std::make_pair(m_id,std::chrono::seconds(1)),std::chrono::duration_cast<FusionTimeMeasurable::value_type>(pfr::profiler_clock::now() - nowOp))

			return std::move(fusionFrame);
		}
	}
	else
	{
		m_fusionProvider.leaveCriticalSection<ddk::IAccessProvider::READING>(std::move(fusionCS));
	}

	return FusionedFrame{};
}
void FrameFusionCollector::checkForNewEncodedFrames()
{
	const pfr::profiler_time_point nowOp = pfr::profiler_clock::now();

	pthread_mutex_lock(&m_mutex);

	while (m_encodedFrames.empty() == false)
	{
		encoded_frame_dist_ref currEncodedFrame = m_encodedFrames.pop();

		pthread_mutex_unlock(&m_mutex);

		const mcu_frame_time_point encodingTime = currEncodedFrame->get_time_stamp();

		if(encodingTime >= m_currEncodingTime)
		{
			m_currEncodingTime = encodingTime;
			
			try
			{
				IFrameWriter::send_result sendRes = m_frameWriter->send(*currEncodedFrame);

				if (sendRes)
				{
					ddk::atomic_post_increment(m_numSentFrames);

					PUBLISH_MEASURE(k_fusion_profile_module,SendingTimeMeasurable,std::make_pair(m_id,std::chrono::seconds(1)),std::chrono::duration_cast<SendingTimeMeasurable::value_type>(pfr::profiler_clock::now() - nowOp));
				}
				else
				{
					DDK_LOG_ERROR("Error sending encoded frame");
				}
			}
			catch(const std::exception& i_excp)
			{
				DDK_LOG_ERROR("Exception sending encoded frame: " << i_excp.what());
			}
		}

		pthread_mutex_lock(&m_mutex);
	}

	pthread_mutex_unlock(&m_mutex);
}
frame_fusioned_fps FrameFusionCollector::checkFusionedFrameRate(const pfr::profiler_time_point& i_time)
{
	const float fps = (m_lastFusionedUpdateTime != pfr::profiler_time_point{}) ? static_cast<float>(m_numFusionedFrames.get()) / std::chrono::duration_cast<std::chrono::milliseconds>(i_time - m_lastFusionedUpdateTime).count() : m_numFusionedFrames.get();

	m_numFusionedFrames.set(0);
	m_lastFusionedUpdateTime = i_time;

	return { m_id, fps * 1000 };
}
frame_sent_fps FrameFusionCollector::checkSentFrameRate(const pfr::profiler_time_point& i_time)
{
	const float fps = (m_lastFusionedUpdateTime != pfr::profiler_time_point{}) ? static_cast<float>(m_numSentFrames.get()) / std::chrono::duration_cast<std::chrono::milliseconds>(i_time - m_lastSentUpdateTime).count() : m_numSentFrames.get();

	m_numSentFrames.set(0);
	m_lastSentUpdateTime = i_time;

	return { m_id, fps * 1000.f };
}

}