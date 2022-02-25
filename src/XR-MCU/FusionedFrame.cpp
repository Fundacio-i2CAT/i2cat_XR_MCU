#include "FusionedFrame.h"
#include "DecodedFrame.h"

namespace mcu
{

FusionedFrame::FusionedFrame(frame_dest_id i_id,detail::IFusionedFrameImpl* i_impl,const ddk::resource_deleter_interface& i_deleter)
: m_id(i_id)
, m_impl(i_impl)
, m_deleter(&i_deleter)
{
}
FusionedFrame::FusionedFrame(frame_dest_id i_id,detail::IFusionedFrameImpl* i_impl,const ddk::resource_deleter_interface& i_deleter, const mcu_frame_time_point& i_timeStamp)
: m_id(i_id)
, m_impl(i_impl)
, m_deleter(&i_deleter)
, m_timeStamp(i_timeStamp)
{
}
FusionedFrame::FusionedFrame(FusionedFrame&& other)
: m_id(std::move(other.m_id))
, m_impl(nullptr)
, m_deleter(nullptr)
, m_timeStamp(std::move(other.m_timeStamp))
, m_srcTimeStamps(std::move(other.m_srcTimeStamps))
{
	std::swap(m_impl,other.m_impl);
	std::swap(m_deleter,other.m_deleter);
}
FusionedFrame::~FusionedFrame()
{
	if (m_deleter)
	{
		m_deleter->deallocate(m_impl);
	}
	else
	{
		delete m_impl;
	}
}
FusionedFrame& FusionedFrame::operator=(FusionedFrame&& other)
{
	if (m_impl)
	{
		if (m_deleter)
		{
			m_deleter->deallocate(m_impl);
		}
		else
		{
			delete m_impl;
		}
		
		m_impl = nullptr;
		m_deleter = nullptr;
	}

	std::swap(m_id,other.m_id);
	std::swap(m_impl,other.m_impl);
	std::swap(m_deleter,other.m_deleter);
	m_timeStamp = std::move(other.m_timeStamp);
	m_srcTimeStamps = std::move(other.m_srcTimeStamps);

	return *this;
}
bool FusionedFrame::merge(frame_source_id i_id, const DecodedFrame& i_frame)
{
	if (const detail::IDecodedFrameImpl* frameImpl = i_frame.get_impl())
	{
		if(m_impl->merge(frameImpl,i_id,i_frame.get_time_stamp()))
		{
			m_srcTimeStamps.push_back(std::make_pair(i_id,i_frame.get_time_stamp()));

			m_timeStamp = mcu_frame_clock::now();

			return true;
		}
	}

	return false;
}
frame_dest_id FusionedFrame::get_id() const
{
	return m_id;
}
mcu_frame_time_point FusionedFrame::get_time_stamp() const
{
	return m_timeStamp;
}
const frame_souce_time_stamp_list& FusionedFrame::get_src_time_stamps() const
{
	return m_srcTimeStamps;
}
FusionedFrame::operator bool() const
{
	return m_impl != nullptr;
}

}