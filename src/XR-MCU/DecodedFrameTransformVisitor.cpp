#include "DecodedFrameTransformVisitor.h"
#include "CwipcDecodedFrame.h"
#include <chrono>



namespace mcu
{
	
DecodedFrameTransformVisitor::DecodedFrameTransformVisitor(const transformation& i_transform)
: m_transform(i_transform) 
{
}


void DecodedFrameTransformVisitor::visit(CwipcDecodedFrame& i_frame)
{
	i_frame.update_bounding_box(m_transform.in_place_transform(i_frame.begin(),i_frame.end()));
}

}