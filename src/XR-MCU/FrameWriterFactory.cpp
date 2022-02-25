#include "FrameWriterFactory.h"
#include "ddk_reference_wrapper.h"
#include "B2DFrameWriter.h"
#include "SocketIoFrameWriter.h"

namespace mcu
{

frame_writer_unique_ref FrameWriterFactory::create_frame_writer(frame_dest_id i_id,const encoder_context& i_context) const
{
	switch (i_context.m_sendSystem)
	{
		case FrameWriterType::B2D:
		{
			return ddk::make_unique_reference<B2DFrameWriter>(i_context.exchangeName,i_context.publishUrl);
		}
		case FrameWriterType::SockettIo:
		{
			return ddk::make_unique_reference<SocketIoFrameWriter>(i_id);
		}
		default:
		{
			throw FusionSenderException{};
		}
	}
}

}