
#include "BasicExceptions.h"

namespace mcu
{
namespace detail
{

template<typename Frame>
bool FrameComparator<Frame>::operator()(const mcu_frame_time_point& i_lhs, const mcu_frame_time_point& i_rhs) const
{
	return i_lhs < i_rhs;
}

}

template<typename Frame>
FusionDataContainer<Frame>::FusionDataContainer(size_t i_maxNumPendingFrames)
: m_maxNumFrames(i_maxNumPendingFrames)
{
}
template<typename Frame>
FusionDataContainer<Frame>::FusionDataContainer(FusionDataContainer&& other)
: m_decodedFrameContainer(std::move(other.m_decodedFrameContainer))
, m_maxNumFrames(other.m_maxNumFrames)
{
}
template<typename Frame>
void FusionDataContainer<Frame>::add_frame(const mcu_frame_time_point& i_timeStamp,Frame i_frame)
{
	if (m_decodedFrameContainer.size() >= m_maxNumFrames)
	{
		m_decodedFrameContainer.pop();
	}

	m_decodedFrameContainer.insert(std::make_pair(i_timeStamp,std::move(i_frame)));
}
template<typename Frame>
void FusionDataContainer<Frame>::clear()
{
	m_decodedFrameContainer.clear();
}
template<typename Frame>
Frame FusionDataContainer<Frame>::pop()
{
	if (m_decodedFrameContainer.empty())
	{
		throw BadAccessException{};
	}

	return m_decodedFrameContainer.pop();
}
template<typename Frame>
typename FusionDataContainer<Frame>::const_iterator FusionDataContainer<Frame>::last() const
{
	return std::next(m_decodedFrameContainer.rbegin()).base();
}
template<typename Frame>
typename FusionDataContainer<Frame>::const_iterator FusionDataContainer<Frame>::end() const
{
	return m_decodedFrameContainer.end();
}
template<typename Frame>
bool FusionDataContainer<Frame>::empty() const
{
	return m_decodedFrameContainer.empty();
}
template<typename Frame>
size_t FusionDataContainer<Frame>::size() const
{
	return m_decodedFrameContainer.size();
}

}