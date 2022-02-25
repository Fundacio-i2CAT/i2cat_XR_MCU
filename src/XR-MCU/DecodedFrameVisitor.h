#pragma once

namespace mcu
{

class CwipcDecodedFrame;

class DecodedFrameVisitor
{
public:
	DecodedFrameVisitor() = default;
	virtual ~DecodedFrameVisitor() = default;

	virtual void visit(CwipcDecodedFrame& i_frame) = 0;
};

}