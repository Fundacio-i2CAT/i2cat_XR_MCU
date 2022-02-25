#include "EncodedFrame.h"

namespace mcu
{

EncodedFrame::EncodedFrame(frame_dest_id i_id, mcu_frame_time_point i_timeStamp, detail::IEncodedFrameImpl* i_impl)
: m_id(i_id)
, m_timeStamp(i_timeStamp)
, m_impl(i_impl)
{
}
EncodedFrame::~EncodedFrame()
{
	if (m_impl)
	{
		delete m_impl;
	}
}
EncodedFrame& EncodedFrame::operator=(EncodedFrame&& other)
{
	if (m_impl)
	{
		delete m_impl;
		m_impl = nullptr;
	}

	std::swap(m_id,other.m_id);
	std::swap(m_timeStamp,other.m_timeStamp);
	std::swap(m_impl,other.m_impl);

	return *this;
}
void* EncodedFrame::get_buffer() const
{
	return (m_impl) ? m_impl->get_buffer() : nullptr;
}
size_t EncodedFrame::get_size() const
{
	return (m_impl) ? m_impl->get_size() : 0;
}
frame_dest_id EncodedFrame::get_id() const
{
	return m_id;
}
mcu_frame_time_point EncodedFrame::get_time_stamp() const
{
	return m_timeStamp;
}

}