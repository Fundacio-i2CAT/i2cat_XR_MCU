#include "FrameEncoderDefs.h"

namespace mcu
{

encoding_time::encoding_time(frame_dest_id i_id,pfr::profiler_time_point i_reference)
: m_id(i_id)
, m_duration(std::chrono::duration_cast<std::chrono::milliseconds>(pfr::profiler_clock::now() - i_reference))
{
}
encoding_time::encoding_time(frame_dest_id i_id,mcu_frame_time_point i_reference)
: m_id(i_id)
, m_duration(std::chrono::duration_cast<std::chrono::milliseconds>(mcu_frame_clock::now() - i_reference))
{
}
frame_dest_id encoding_time::get_id() const
{
	return m_id;
}
std::chrono::milliseconds encoding_time::get_duration() const
{
	return m_duration;
}

frame_fusioned_fps::frame_fusioned_fps(frame_dest_id i_id, float i_fps)
: m_id(i_id)
, m_fps(i_fps)
{
}
frame_dest_id frame_fusioned_fps::get_id() const
{
	return m_id;
}
float frame_fusioned_fps::get_fps() const
{
	return m_fps;
}

frame_sent_fps::frame_sent_fps(frame_dest_id i_id, float i_fps)
: m_id(i_id)
, m_fps(i_fps)
{
}
frame_dest_id frame_sent_fps::get_id() const
{
	return m_id;
}
float frame_sent_fps::get_fps() const
{
	return m_fps;
}

}