#pragma once

#include "ddk_signal.h"
#include "FrameDefs.h"

namespace mcu
{

class IRawFrameSource
{
public:
	ddk::signal<void(frame_data)> sig_onNewFrame;

	virtual ~IRawFrameSource() = default;
};

typedef ddk::lent_reference_wrapper<IRawFrameSource> iraw_frame_source_lent_ref;
typedef ddk::lent_reference_wrapper<const IRawFrameSource> iraw_frame_source_const_lent_ref;

}