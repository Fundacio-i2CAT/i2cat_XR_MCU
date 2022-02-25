#include "CwipcFrameDecoder.h"
#include "cwipc_util/api_pcl.h"
#include "cwipc_util/api.h"
#include "cwipc_codec/api.h"
#include "CwipcDecodedFrame.h"
#include <chrono>

namespace mcu
{
namespace detail
{

cwipc_decoder* create_decoder()
{
	char* message = NULL;

	if(cwipc_decoder* res = cwipc_new_decoder(&message,CWIPC_API_VERSION))
	{
		DDK_LOG_INFO("PCDecoder: cwipc_new_decoder created successfully!\n");

		return res;
	}
	else
	{
		DDK_FAIL_OR_LOG("PCDecoder: cwipc_new_decoder creation failed\n"); // Should not happen, should throw exception

		return nullptr;
	}
}

cwiDecoder::~cwiDecoder()
{
	if(cwipc_decoder* encoder = get_encoder())
	{
		encoder->free();
	}
}
void cwiDecoder::feed(char* i_data,size_t i_size)
{
	if(cwipc_decoder* encoder = get_encoder())
	{
		encoder->feed(i_data,i_size);
	}
}
bool cwiDecoder::available(bool i_blocking)
{
	if(cwipc_decoder* encoder = get_encoder())
	{
		return encoder->available(i_blocking);
	}
	else
	{
		return false;
	}
}
cwipc* cwiDecoder::get_data()
{
	if(cwipc_decoder* decoder = get_encoder())
	{
		return decoder->get();
	}
	else
	{
		return false;
	}
}
cwipc_decoder* cwiDecoder::get_encoder_impl()
{
	return get_encoder();
}
cwipc_decoder* cwiDecoder::get_encoder()
{
	static thread_local cwipc_decoder* s_cwipcEncoder = create_decoder();

	return s_cwipcEncoder;
}

}

CwipcFrameDecoder::CwipcFrameDecoder(frame_source_id i_id, idecoded_frame_receiver_lent_ref i_frameReceiver, ddk::thread_sheaf i_threadSheaf, size_t i_maxPendingFrames)
: FrameDecoderBase(i_id,i_frameReceiver,std::move(i_threadSheaf),i_maxPendingFrames)
{
}
CwipcFrameDecoder::decode_result CwipcFrameDecoder::processFrameData(const frame_data& i_frame)
{
	m_decoder.feed(const_cast<char*>(i_frame.second.data()),i_frame.second.size());
	if (m_decoder.available(true))
	{
		if(cwipc* decodedData = m_decoder.get_data())
		{
			const time_t frameMs = decodedData->timestamp();
			const time_t frameS = frameMs / 1000;

			return DecodedFrame::create_impl<CwipcDecodedFrame>(mcu_frame_clock::from_time_t(frameS) + std::chrono::milliseconds(frameMs - frameS * 1000),decodedData);
		}
		else
		{
			return ddk::make_error<decode_result>(DecodeFrameError::DecoderError);
		}
	}
	else
	{
		return ddk::make_error<decode_result>(DecodeFrameError::DecoderNotAvailable);
	}
}

}