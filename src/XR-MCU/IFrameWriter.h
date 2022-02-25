#pragma once

#include "ddk_result.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "EncodedFrame.h"

namespace mcu
{

class IFrameWriter
{
public:
	enum class WriterSendFrameErrorCode
	{
		SendError,
		NotAvailable,
		EmptyBuffer
	};
	typedef ddk::result<size_t,WriterSendFrameErrorCode> send_result;

	virtual ~IFrameWriter() = default;
	virtual send_result send(const EncodedFrame&) = 0;
};

typedef ddk::unique_reference_wrapper<IFrameWriter> frame_writer_unique_ref;
typedef ddk::unique_reference_wrapper<const IFrameWriter> frame_writer_const_unique_ref;
typedef ddk::unique_pointer_wrapper<IFrameWriter> frame_writer_unique_ptr;
typedef ddk::unique_pointer_wrapper<const IFrameWriter> frame_writer_const_unique_ptr;
typedef ddk::lent_reference_wrapper<IFrameWriter> frame_writer_lent_ref;
typedef ddk::lent_reference_wrapper<const IFrameWriter> frame_writer_const_lent_ref;

}