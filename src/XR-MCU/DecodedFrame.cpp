#include "DecodedFrame.h"
#include <utility>

namespace mcu
{
namespace detail
{

void DecodedFrameImpl::update_bounding_box(const bounding_box& i_boundingBox)
{
	m_boundingBox = i_boundingBox;
}
bounding_box DecodedFrameImpl::get_bounding_box() const
{
	return m_boundingBox;
}

}

DecodedFrame::DecodedFrame(DecodedFrame&& other)
: m_impl(nullptr)
, m_timeStamp(other.m_timeStamp)
{
	std::swap(m_impl,other.m_impl);
}
DecodedFrame::DecodedFrame(const mcu_frame_time_point& i_timestamp, detail::IDecodedFrameImpl* i_impl)
: m_timeStamp(i_timestamp)
, m_impl(i_impl)
{
}
DecodedFrame::~DecodedFrame()
{
	if (m_impl)
	{
		delete m_impl;
	}
}
DecodedFrame& DecodedFrame::operator=(DecodedFrame&& other)
{
	if (m_impl)
	{
		delete m_impl;
		m_impl = nullptr;
	}

	std::swap(m_impl,other.m_impl);
	m_timeStamp = other.m_timeStamp;

	return *this;
}
bounding_box DecodedFrame::get_bounding_box() const
{
	return (m_impl) ? m_impl->get_bounding_box() : bounding_box();
}
mcu_frame_time_point DecodedFrame::get_time_stamp() const
{
	return m_timeStamp;
}
size_t DecodedFrame::size() const
{
	return (m_impl) ? m_impl->size() : 0;
}
bool DecodedFrame::empty() const
{
	return (m_impl) ? m_impl->empty() : true;
}
const detail::IDecodedFrameImpl* DecodedFrame::get_impl() const
{
	return m_impl;
}
void DecodedFrame::visit(DecodedFrameVisitor& i_visitor) const
{
	if (m_impl)
	{
		m_impl->visit(i_visitor);
	}
	else
	{
		DDK_FAIL_OR_LOG("Trying to transform empty decoded frame!");
	}
}
}