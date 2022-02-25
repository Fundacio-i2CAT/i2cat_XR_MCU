#pragma once

#include "IFusionedFrameEncoder.h"
#include "ddk_task_executor.h"

namespace mcu
{

class FrameEncoderFactory
{
public:
	ifusioned_frame_encoder_unique_ref create_frame_encoder(EncodecType i_type, frame_dest_id i_id, ddk::task_executor_lent_ref i_taskExecutor, size_t i_maxPendingDecodedFrames);

};

}