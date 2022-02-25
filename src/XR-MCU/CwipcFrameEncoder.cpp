#include "CwipcFrameEncoder.h"
#include "CwipcEncodedFrame.h"
#include "CwipcFusionedFrame.h"
#include <ctime>
#include "IMCUProfiler.h"

namespace mcu
{
namespace detail
{

cwipc_encoder* create_encoder()
{
	cwipc_encoder_params param;
	param.do_inter_frame = false;
	param.gop_size = 1;
	param.exp_factor = 1.0;
	param.octree_bits = 9;
	param.jpeg_quality = 85;
	param.macroblock_size = 16;
	param.tilenumber = 0;
	param.voxelsize = 0.005;

	char* errorString;

	return cwipc_new_encoder(CWIPC_ENCODER_PARAM_VERSION, &param, &errorString, CWIPC_API_VERSION);
}

cwiEncoder::~cwiEncoder()
{
	if (cwipc_encoder* encoder = get_encoder())
	{
		encoder->free();
	}
}
void cwiEncoder::feed(cwipc* i_data)
{
	if (cwipc_encoder* encoder = get_encoder())
	{
		encoder->feed(i_data);
	}
}
bool cwiEncoder::available(bool i_blocking)
{
	if (cwipc_encoder* encoder = get_encoder())
	{
		return encoder->available(i_blocking);
	}
	else
	{
		return false;
	}
}
cwipc_encoder* cwiEncoder::get_encoder_impl()
{
	return get_encoder();
}
cwipc_encoder* cwiEncoder::get_encoder()
{
	static thread_local cwipc_encoder* s_cwipcEncoder = create_encoder();

	return s_cwipcEncoder;
}

}

CwipcFrameEncoder::CwipcFrameEncoder(frame_dest_id i_id, ddk::task_executor_lent_ref i_taskExecutor, size_t i_maxPendingDecodedFrames)
: FrameEncoderBase(i_id,i_taskExecutor,i_maxPendingDecodedFrames)
{
}
encoded_frame_dist_ref CwipcFrameEncoder::processFrameData(const ddk::function<FusionedFrame(const frame_souce_time_stamp_list&)>& i_frameFusioner, const frame_souce_time_stamp_list& i_frameList)
{
	const FusionedFrame fusionFrame = ddk::eval(i_frameFusioner,i_frameList);

	if (CwipcFusionedFrame* decodedFrame = fusionFrame.get_impl<CwipcFusionedFrame>())
	{
		m_encoder.feed(decodedFrame->getData());

		if (m_encoder.available(true))
		{
			try
			{
				return EncodedFrame::create_impl<CwipcEncodedFrame>(fusionFrame.get_id(),fusionFrame.get_time_stamp(),m_encoder.get_encoder_impl());
			}
			catch (const FusionEncoderException& i_excp)
			{
				throw i_excp;
			}
		}
	}

	throw FusionEncoderException{};
}

}