#include "FrameEncoderFactory.h"
#include "CwipcFrameEncoder.h"
#include "ddk_task_executor.h"

namespace mcu
{

ifusioned_frame_encoder_unique_ref FrameEncoderFactory::create_frame_encoder(EncodecType i_type,frame_dest_id i_id, ddk::task_executor_lent_ref i_taskExecutor, size_t i_maxPendingDecodedFrames)
{
	switch (i_type)
	{
		case EncodecType::Cwipc:
		{
			return ddk::make_unique_reference<CwipcFrameEncoder>(i_id,i_taskExecutor,i_maxPendingDecodedFrames);
		}
		default:
		{
			throw FusionEncoderException{};
		}
	}
}

}