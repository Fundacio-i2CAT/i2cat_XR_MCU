#include "FrameDefs.h"

namespace mcu
{
namespace detail
{

std::chrono::system_clock::duration reference_time::m_offset = std::chrono::seconds(0);

std::chrono::system_clock::time_point reference_time::now()
{
	return std::chrono::system_clock::now() - m_offset;
}
void reference_time::calibrate(const std::chrono::system_clock::time_point& i_now)
{
	m_offset = std::chrono::system_clock::now() - i_now;
}

}

const frame_source_id k_invalidFrameSourceId = frame_source_id("");

bool frame_souce_time_stamp_list_less_operator::operator()(const mcu_frame_time_point_list& i_lhs,const mcu_frame_time_point_list& i_rhs) const
{
	if(i_lhs.size() != i_rhs.size())
	{
		return i_lhs.size() < i_rhs.size();
	}
	else
	{
		{
			mcu_frame_time_point_list::const_iterator itLhs = i_lhs.begin();
			mcu_frame_time_point_list::const_iterator itRhs = i_rhs.begin();
			for(; itLhs != i_lhs.end() && itRhs != i_rhs.end(); ++itLhs,++itRhs)
			{
				if(*itLhs >= *itRhs)
				{
					return false;
				}
			}
		}

		return true;
	}
}

}