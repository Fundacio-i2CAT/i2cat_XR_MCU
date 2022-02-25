#pragma once

#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "FrameDecoderDefs.h"
#include "ddk_signal.h"
#include "ddk_thread.h"
#include "ddk_optional.h"

class cwipc_decoder;

namespace mcu
{

class IRawFrameDecoder
{
public:
	ddk::signal<void(frame_id,cwipc_decoder*)> sig_onNewDecodedFrame;
	ddk::signal<void()> sig_onPerformanceNeedsAttention;

	virtual ~IRawFrameDecoder() = default;

	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void set_affinity(const cpu_set_t&) = 0;
	virtual ddk::optional<ddk::thread> borrow_thread() = 0;
	virtual void lend_thread(ddk::thread) = 0;
	virtual void enqueue_raw_frame(frame_data) = 0;
};

typedef ddk::unique_reference_wrapper<IRawFrameDecoder> iraw_frame_decoder_unique_ref;
typedef ddk::unique_reference_wrapper<const IRawFrameDecoder> iraw_frame_decoder_const_unique_ref;
typedef ddk::unique_pointer_wrapper<IRawFrameDecoder> iraw_frame_decoder_unique_ptr;
typedef ddk::unique_pointer_wrapper<const IRawFrameDecoder> iraw_frame_decoder_const_unique_ptr;
typedef ddk::lent_reference_wrapper<IRawFrameDecoder> iraw_frame_decoder_lent_ref;
typedef ddk::lent_reference_wrapper<const IRawFrameDecoder> iraw_frame_decoder_const_lent_ref;

}