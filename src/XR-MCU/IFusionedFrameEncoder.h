#pragma once

#include "EncodedFrame.h"
#include "IMCUProfiler.h"
#include "DurationMeasurable.h"
#include "FusionedFrame.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_async.h"
#include <functional>

namespace mcu
{

class IFusionedFrameEncoder
{
public:
	virtual ~IFusionedFrameEncoder() = default;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual ddk::shared_future<encoded_frame_dist_ref> encode_frame(const ddk::function<FusionedFrame(const frame_souce_time_stamp_list&)>&, const frame_souce_time_stamp_list&) = 0;
};

typedef ddk::unique_reference_wrapper<IFusionedFrameEncoder> ifusioned_frame_encoder_unique_ref;
typedef ddk::unique_reference_wrapper<const IFusionedFrameEncoder> ifusioned_frame_encoder_const_unique_ref;
typedef ddk::unique_pointer_wrapper<IFusionedFrameEncoder> ifusioned_frame_encoder_unique_ptr;
typedef ddk::unique_pointer_wrapper<const IFusionedFrameEncoder> ifusioned_frame_encoder_const_unique_ptr;
typedef ddk::lent_pointer_wrapper<IFusionedFrameEncoder> ifusioned_frame_encoder_lent_ptr;
typedef ddk::lent_pointer_wrapper<const IFusionedFrameEncoder> ifusioned_frame_encoder_const_lent_ptr;
typedef ddk::lent_reference_wrapper<IFusionedFrameEncoder> ifusioned_frame_encoder_lent_ref;
typedef ddk::lent_reference_wrapper<const IFusionedFrameEncoder> ifusioned_frame_encoder_const_lent_ref;

}