#pragma once

#include "ddk_type_id.h"
#include <list>
#include <chrono>

namespace mcu
{
namespace detail
{

struct reference_time : public std::chrono::system_clock
{
public:
	using std::chrono::system_clock::system_clock;

	static std::chrono::system_clock::time_point now();

	static void calibrate(const std::chrono::system_clock::time_point& i_now);

private:
	static std::chrono::system_clock::duration m_offset;
};

}

struct frame_id_t;
typedef ddk::Id<size_t,frame_id_t> frame_id;

struct frame_source_id_t;
typedef ddk::Id<std::string,frame_id_t> frame_source_id;
typedef std::set<frame_source_id> frame_source_list_id;


typedef detail::reference_time mcu_frame_clock;
typedef mcu_frame_clock::time_point mcu_frame_time_point;
typedef std::vector<mcu_frame_clock::time_point> mcu_frame_time_point_list;

typedef std::string frame_arena;
typedef std::pair<mcu_frame_time_point,frame_arena> frame_data;

struct frame_souce_time_stamp_list_less_operator
{
	bool operator()(const mcu_frame_time_point_list& i_lhs,const mcu_frame_time_point_list& i_rhs) const;
};

extern const frame_source_id k_invalidFrameSourceId;

}