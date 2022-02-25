#pragma once

#include "FrameEncoderBase.h"
#include "cwipc_util/api_pcl.h"
#include "cwipc_codec/api.h"

namespace mcu
{
namespace detail
{

struct cwiEncoder
{
public:
	cwiEncoder() = default;
	~cwiEncoder();
	void feed(cwipc* i_data);
	bool available(bool i_blocking);
	cwipc_encoder* get_encoder_impl();

private:
	cwipc_encoder* get_encoder();
};

}

class CwipcFrameEncoder : public FrameEncoderBase
{
public:
	CwipcFrameEncoder(frame_dest_id i_id, ddk::task_executor_lent_ref i_taskExecutor, size_t i_maxPendingDecodedFrames);

private:
	encoded_frame_dist_ref processFrameData(const ddk::function<FusionedFrame(const frame_souce_time_stamp_list&)>& i_frameFusioner,const frame_souce_time_stamp_list& i_frameList) override;

	detail::cwiEncoder m_encoder;
};

}