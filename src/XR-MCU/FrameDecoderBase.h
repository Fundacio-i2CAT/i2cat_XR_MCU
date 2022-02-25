#pragma once

#include "IRawFrameDecoder.h"
#include "ddk_lock_free_stack.h"
#include "ddk_thread_pool.h"
#include "ddk_atomics.h"
#include "DecodedFrame.h"
#include "IDecodedFrameReceiver.h"
#include "ProfilerDefs.h"

namespace mcu
{

class FrameDecoderBase : public IRawFrameDecoder
{
	static const size_t k_numDecodedFramesToStartMeasuringFrameRate = 30;

public:
	FrameDecoderBase(frame_source_id i_id, idecoded_frame_receiver_lent_ref i_frameReceiver, ddk::thread_sheaf i_threadSheaf, size_t i_maxPendingFrames);
	~FrameDecoderBase();

protected:
	enum class DecodeFrameError
	{
		DecoderNotAvailable,
		DecoderError,
		DecoderEmptyFrame
	};
	typedef ddk::result<DecodedFrame,DecodeFrameError> decode_result;

private:
	enum class State
	{
		Idle,
		Running
	};

	void start() override;
	void stop() override;
	void set_affinity(const cpu_set_t&) override;
	void enqueue_raw_frame(frame_data) override;
	ddk::optional<ddk::thread> borrow_thread() override;
	void lend_thread(ddk::thread) override;

	virtual decode_result processFrameData(const frame_data&) = 0;

	void processNextFrameData();
	frame_decoder_fps compute_frame_rate(const pfr::profiler_time_point& i_time);

	frame_source_id m_id;
	idecoded_frame_receiver_lent_ref m_frameReceiver;
	ddk::thread_sheaf m_threadSheaf;
	ddk::single_consumer_lock_free_stack<frame_data> m_pendingFramesStack;
	size_t m_maxPendingFrames;
	ddk::atomic_size_t m_pendingFrames;
	State m_state;
	size_t m_numDecodedFrames;
	pfr::measure_id m_decodedFrameRateMeasureId;
	pfr::profiler_time_point m_lastFrameRateCheckTime;
};

}

#include "FrameDecoderBase.inl"