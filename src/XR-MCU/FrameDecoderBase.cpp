#include "FrameDecoderBase.h"
#include "IMCUProfiler.h"
#include "OperationsRateMeasurable.h"
#include "AcquisitionModuleProfiler.h"
#include "MCUManagerDefs.h"
#include <ctime>

namespace mcu
{

FrameDecoderBase::FrameDecoderBase(frame_source_id i_id, idecoded_frame_receiver_lent_ref i_frameReceiver, ddk::thread_sheaf i_threadSheaf, size_t i_maxPendingFrames)
: m_id(i_id)
, m_frameReceiver(i_frameReceiver)
, m_threadSheaf(std::move(i_threadSheaf))
, m_maxPendingFrames(i_maxPendingFrames)
, m_pendingFrames(0)
, m_numDecodedFrames(0)
, m_state(State::Idle)
, m_decodedFrameRateMeasureId(pfr::k_invalid_measure_id)
{
}
FrameDecoderBase::~FrameDecoderBase()
{
	STOP_MEASURE(k_acquisition_profile_module,m_decodedFrameRateMeasureId);
}
void FrameDecoderBase::start()
{
	if (m_state == State::Idle)
	{
		m_state = State::Running;
	}
	else
	{
		DDK_FAIL("Attempting to start decoder already started");
	}
}
void FrameDecoderBase::stop()
{
	if (m_state == State::Running)
	{
		m_threadSheaf.stop();

		m_state = State::Idle;
	}
	else
	{
		DDK_FAIL("Attempting to stop decoder non started");
	}
}
void FrameDecoderBase::set_affinity(const cpu_set_t& i_cpuSet)
{
	ddk::thread_sheaf::iterator itThread = m_threadSheaf.begin();
	for(; itThread != m_threadSheaf.end(); ++itThread)
	{
		itThread->set_affinity(i_cpuSet);
	}
}
void FrameDecoderBase::enqueue_raw_frame(frame_data i_frame)
{
	if (m_state == State::Running)
	{
		if (m_pendingFrames.get() > m_maxPendingFrames)
		{
			m_pendingFramesStack.pop();
		}
		else
		{
			ddk::atomic_pre_increment(m_pendingFrames);
		}

		m_pendingFramesStack.push(std::move(i_frame));

		m_threadSheaf.start(ddk::make_function(this,&FrameDecoderBase::processNextFrameData),1);
	}
}
ddk::optional<ddk::thread> FrameDecoderBase::borrow_thread()
{
	return m_threadSheaf.extract();
}
void FrameDecoderBase::lend_thread(ddk::thread i_thread)
{
	m_threadSheaf.insert(std::move(i_thread));

	i_thread.start(ddk::make_function(this,&FrameDecoderBase::processNextFrameData));
}
void FrameDecoderBase::processNextFrameData()
{
	while (ddk::optional<frame_data> newFrame = m_pendingFramesStack.pop())
	{
		if (m_decodedFrameRateMeasureId == pfr::k_invalid_measure_id &&
			m_numDecodedFrames > k_numDecodedFramesToStartMeasuringFrameRate)
		{
			m_numDecodedFrames = 0;

			START_MEASURE(k_acquisition_profile_module,pfr::make_measurable<DecodedFrameRateMeasurable>(ddk::make_function(this,&FrameDecoderBase::compute_frame_rate),std::chrono::seconds(1)),m_decodedFrameRateMeasureId)
		}

		m_numDecodedFrames++;

		ddk::atomic_pre_decrement(m_pendingFrames);

		const pfr::profiler_time_point beforeOp = pfr::profiler_clock::now();

		decode_result decodeRes = processFrameData(*newFrame);

		PUBLISH_MEASURE(k_acquisition_profile_module,DecodingTimeMeasurable,std::make_pair(m_id,std::chrono::seconds(1)),m_id,beforeOp)

		if (decodeRes)
		{
			DecodedFrame dcodedFrame = std::move(decodeRes).extract();

			PUBLISH_MEASURE(k_acquisition_profile_module,PointcloudSizeMeasurable,std::make_pair(m_id,std::chrono::seconds(1)),m_id,dcodedFrame.size());

			PUBLISH_MEASURE(k_acquisition_profile_module,DecodedFrameLatencyMeasurable,std::make_pair(m_id,std::chrono::seconds(1)),m_id,dcodedFrame.get_time_stamp());

			m_frameReceiver->enqueueDecodedFrame(m_id,std::move(dcodedFrame));
		}
	}
}
frame_decoder_fps FrameDecoderBase::compute_frame_rate(const pfr::profiler_time_point& i_time)
{
	const float frameRate = (m_lastFrameRateCheckTime != pfr::profiler_time_point()) ? static_cast<float>(m_numDecodedFrames) / std::chrono::duration_cast<std::chrono::milliseconds>(i_time - m_lastFrameRateCheckTime).count() : m_numDecodedFrames;

	m_numDecodedFrames = 0;
	m_lastFrameRateCheckTime = i_time;

	return {m_id,frameRate * 1000.f};
}

}