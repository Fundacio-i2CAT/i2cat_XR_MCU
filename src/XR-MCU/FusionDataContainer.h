#pragma once

#include "FusionDefs.h"
#include "FrameDefs.h"
#include <map>

namespace mcu
{
namespace detail
{

template<typename Frame>
struct FrameComparator
{
	bool operator()(const mcu_frame_time_point& i_lhs, const mcu_frame_time_point& i_rhs) const;
};

template<typename Frame>
using priority_frame_container = std::map<mcu_frame_time_point,Frame,detail::FrameComparator<Frame>>;

template<typename Frame>
struct ordered_frame_conatiner : public priority_frame_container<Frame>
{
private:
	typedef typename priority_frame_container<Frame>::iterator iterator;

public:
	using priority_frame_container<Frame>::begin;
	using priority_frame_container<Frame>::end;
	using priority_frame_container<Frame>::clear;
	using priority_frame_container<Frame>::size;
	using priority_frame_container<Frame>::empty;

	typedef typename priority_frame_container<Frame>::const_iterator const_iterator;

	inline Frame pop()
	{
		iterator itFirst = priority_frame_container<Frame>::begin();

		Frame res = std::move(itFirst->second);

		priority_frame_container<Frame>::erase(itFirst);

		return std::move(res);
	}
};

}

template<typename Frame>
class FusionDataContainer
{
public:
	typedef detail::ordered_frame_conatiner<Frame> frame_container;
	typedef typename frame_container::const_iterator const_iterator;

	FusionDataContainer(size_t i_maxNumPendingFrames);
	FusionDataContainer(const FusionDataContainer&) = delete;
	FusionDataContainer(FusionDataContainer&& other);

	FusionDataContainer& operator=(const FusionDataContainer&) = delete;
	FusionDataContainer& operator=(FusionDataContainer&&) = delete;
	void add_frame(const mcu_frame_time_point& i_timeStamp, Frame i_frame);
	void clear();
	Frame pop();
	const_iterator last() const;
	const_iterator end() const;
	bool empty() const;
	size_t size() const;

private:
	frame_container m_decodedFrameContainer;
	size_t m_maxNumFrames;
};

}

#include "FusionDataContainer.inl"