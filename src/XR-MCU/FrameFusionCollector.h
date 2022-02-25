#pragma once

#include "ISceneBuilder.h"
#include "IFusionProvider.h"
#include "FusionDefs.h"
#include "MCUConfig.h"
#include "IFrameWriter.h"
#include "ProfilerDefs.h"
#include "FusionDataContainer.h"
#include "IPhysicalObject.h"
#include "FusionedFrame.h"
#include "ddk_lend_from_this.h"
#include "ddk_lock_free_stack.h"
#include "ddk_thread_executor.h"
#include "ddk_reference_wrapper_deleter.h"
#include "ddk_future.h"
#include <functional>
#include <unordered_map>

namespace mcu
{

class FrameFusionCollector : public ddk::resource_deleter_interface, public ISceneBuilder
{
public:
	FrameFusionCollector(frame_dest_id i_id, iphysical_object_const_lent_ptr i_obj, const IFusionProvider& i_fusionProvider, frame_writer_unique_ref i_frameWriter, const McuConfig& i_config);
	FrameFusionCollector(const FrameFusionCollector&) = delete;
	FrameFusionCollector(FrameFusionCollector&& other);
	~FrameFusionCollector();

	FrameFusionCollector& operator=(const FrameFusionCollector&) = delete;
	FrameFusionCollector& operator=(FrameFusionCollector&& other) = delete;

	void start();
	void stop();
	void set_affinity(const cpu_set_t& i_cpusET);

private:
	void deallocate(const void* i_ptr) const override;
	bool add_source(frame_source_id i_id) override;
	bool remove_source(frame_source_id i_id) override;
	size_t size() const override;
	bool empty() const override;
	const transformation& get_transform() const override;
	Frustrum get_frustrum() const override;

	FusionedFrame performFusion(const frame_souce_time_stamp_list& i_scene);
	void onNewEncodedFrameFuture(ddk::shared_future<encoded_frame_dist_ref> i_frame);
	void checkForNewDecodedFrames();
	void checkForNewEncodedFrames();
	frame_fusioned_fps checkFusionedFrameRate(const pfr::profiler_time_point& i_time);
	frame_sent_fps checkSentFrameRate(const pfr::profiler_time_point& i_time);

	const frame_dest_id m_id;
	iphysical_object_const_lent_ptr m_physicalObject;
	const IFusionProvider& m_fusionProvider;
	frame_writer_unique_ref m_frameWriter;
	const size_t m_numEncodedFramesToSent;
	std::chrono::milliseconds m_updateTime;
	frame_source_list_id m_ids;
	ddk::thread_polling_executor m_fusionExecutor;
	ddk::thread_polling_executor m_senderExecutor;
	mcu_frame_time_point m_lastUpdateTime;
	mcu_frame_time_point m_currDecodingTime;
	mcu_frame_time_point m_currEncodingTime;
	mutable ddk::single_consumer_lock_free_stack<FusionedFrame> m_availableFrames;
	bool m_sentStarted;

	//profiling data
	pfr::measure_id m_fusionRateMeasureId;
	pfr::measure_id m_sentRateMeasureId;
	pthread_mutex_t m_mutex;
	pthread_mutex_t m_idListMutex;
	FusionDataContainer<encoded_frame_dist_ref> m_encodedFrames;
	bool m_forceFusion;
	ddk::atomic_size_t m_numFusionedFrames;
	ddk::atomic_size_t m_numSentFrames;
	pfr::profiler_time_point _lastFusionedUpdateTime;
	pfr::profiler_time_point m_lastFusionedUpdateTime;
	pfr::profiler_time_point m_lastSentUpdateTime;
};

typedef ddk::unique_reference_wrapper<FrameFusionCollector> frame_fusion_collector_unique_ref;
typedef ddk::unique_reference_wrapper<const FrameFusionCollector> frame_fusion_collector_const_unique_ref;
typedef ddk::lent_reference_wrapper<FrameFusionCollector> frame_fusion_collector_lent_ref;
typedef ddk::lent_reference_wrapper<const FrameFusionCollector> frame_fusion_collector_const_lent_ref;

}