#pragma once

#include "DecodedFrameVisitor.h"
#include "PlayerTransformation.h"
#include "FusionDefs.h"
#include "PlayerUtils.h"

namespace mcu
{

class DecodedFrameTransformVisitor : public DecodedFrameVisitor
{
public:
	DecodedFrameTransformVisitor(const transformation& i_transform);

private:
	void visit(CwipcDecodedFrame& i_frame);

	const transformation& m_transform;
};

}