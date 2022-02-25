#pragma once

#include "DecodedFrameVisitor.h"
#include "FusionedFrameVisitor.h"
#include "CwipcDecodedFrame.h"
#include "CwipcFusionedFrame.h"

namespace mcu
{

class FusionedFrameCreateVisitor : public DecodedFrameVisitor, public FusionedFrameVisitor
{
public:
	FusionedFrameCreateVisitor(mcu_frame_time_point i_timeStamp);

	detail::IFusionedFrameImpl* extract_impl();

private:
	void visit(CwipcDecodedFrame& i_frame) override;
	void visit(CwipcFusionedFrame& i_frame) override;

	const mcu_frame_time_point m_timeStamp;
	detail::IFusionedFrameImpl* m_impl = nullptr;
};

}