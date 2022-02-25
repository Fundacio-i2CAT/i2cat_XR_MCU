#pragma once

#include "DecodedFrame.h"
#include "ddk_result.h"

namespace mcu
{

class IDecodedFrameReceiver
{
public:
	virtual ~IDecodedFrameReceiver() = default;

	virtual void enqueueDecodedFrame(frame_source_id,DecodedFrame) = 0;
};

typedef ddk::lent_reference_wrapper<IDecodedFrameReceiver> idecoded_frame_receiver_lent_ref;
typedef ddk::lent_reference_wrapper<const IDecodedFrameReceiver> idecoded_frame_receiver_const_lent_ref;

}