#pragma once

#include "FusionDataContainer.h"
#include "ddk_lock_free_stack.h"
#include "DecodedFrame.h"
#include "ddk_thread_executor.h"
#include "IPhysicalObject.h"
#include "IFusionNotifier.h"

namespace mcu
{

class FusionPlayerDataContainer
{
public:
	typedef FusionDataContainer<DecodedFrame>::const_iterator const_iterator;

	FusionPlayerDataContainer(frame_source_id i_id, size_t i_maxNumPendingFrames,iphysical_object_const_lent_ref i_object, const IFusionNotifier& i_notifier);

	void add_decoded_frame(DecodedFrame i_frame);
	bounding_box get_bounding_box() const;
	const_iterator last() const;
	bool empty() const;

private:
	void checkForNewDecodedFrames();
	frame_decoder_fps checkTransformedFrameRate(const pfr::profiler_time_point& i_time);

	const frame_source_id m_id;
	const size_t m_maxNumPendingFrames;
	iphysical_object_const_lent_ref m_object;
	const IFusionNotifier& m_notifier;
	ddk::atomic_size_t m_pendingFrames;
	ddk::single_consumer_lock_free_stack<DecodedFrame> m_pendingFramesToBeTransformed;
	FusionDataContainer<DecodedFrame> m_transformedContainer;
	ddk::thread_event_driven_executor m_transformerExecutor;

	pfr::measure_id m_transformRateMeasureId;
	ddk::atomic_size_t m_numTransformedFrames;
	pfr::profiler_time_point m_lastTransfomedUpdateTime;
};

}