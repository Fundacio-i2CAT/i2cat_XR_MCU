#pragma once

#include "FrameDecoderBase.h"
#include "CwipcDecodedFrame.h"

class cwipc_decoder;

namespace mcu
{
namespace detail
{

struct cwiDecoder
{
public:
	cwiDecoder() = default;
	~cwiDecoder();
	void feed(char* i_data, size_t i_size);
	bool available(bool i_blocking);
	cwipc* get_data();
	cwipc_decoder* get_encoder_impl();

private:
	cwipc_decoder* get_encoder();
};

}

class CwipcFrameDecoder : public FrameDecoderBase
{
public:
	CwipcFrameDecoder(frame_source_id i_id, idecoded_frame_receiver_lent_ref i_frameReceiver, ddk::thread_sheaf i_threadSheaf, size_t i_maxPendingFrames);

private:
	decode_result processFrameData(const frame_data&) override;

	detail::cwiDecoder m_decoder;
};

}