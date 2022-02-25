#pragma once

#include "IFrameWriter.h"

namespace mcu
{

class FrameWriterFactory
{
public:
	FrameWriterFactory() = default;

	frame_writer_unique_ref create_frame_writer(frame_dest_id i_id, const encoder_context& i_context) const;

private:

};

}