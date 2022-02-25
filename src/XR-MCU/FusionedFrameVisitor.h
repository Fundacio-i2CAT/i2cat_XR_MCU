#pragma once

namespace mcu
{

class CwipcFusionedFrame;

class FusionedFrameVisitor
{
public:
	FusionedFrameVisitor() = default;
	virtual ~FusionedFrameVisitor() = default;

	virtual void visit(CwipcFusionedFrame& i_frame) = 0;
};

}