#include "FrameDecoderDefs.h"

namespace mcu
{

pointcloud_size::pointcloud_size(frame_source_id i_id,size_t i_size)
: m_id(i_id)
, m_size(i_size)
{
}
frame_source_id pointcloud_size::get_id() const
{
	return m_id;
}
size_t pointcloud_size::get_size() const
{
	return m_size;
}

frame_receiver_fps::frame_receiver_fps(const std::string& i_id,float i_fps)
: m_id(i_id)
,m_fps(i_fps)
{
}
std::string frame_receiver_fps::get_id() const
{
	return m_id;
}
float frame_receiver_fps::get_fps() const
{
	return m_fps;
}
bool frame_receiver_fps::operator==(const frame_receiver_fps& other) const
{
	return m_fps == other.m_fps;
}
bool frame_receiver_fps::operator<(const frame_receiver_fps& other) const
{
	return m_id < other.m_id;
}

frame_latency::frame_latency(frame_source_id i_id, mcu_frame_time_point i_reference)
: m_id(i_id)
, m_latency(std::chrono::duration_cast<std::chrono::milliseconds>(mcu_frame_clock::now() - i_reference))
{
}
frame_source_id frame_latency::get_id() const
{
	return m_id;
}
std::chrono::milliseconds frame_latency::get_latency() const
{
	return m_latency;
}

decoding_time::decoding_time(frame_source_id i_id, pfr::profiler_time_point i_reference)
: m_id(i_id)
, m_duration(std::chrono::duration_cast<std::chrono::milliseconds>(pfr::profiler_clock::now() - i_reference))
{
}
frame_source_id decoding_time::get_id() const
{
	return m_id;
}
std::chrono::milliseconds decoding_time::get_duration() const
{
	return m_duration;
}

frame_decoder_fps::frame_decoder_fps(frame_source_id i_id, float i_fps)
: m_id(i_id)
, m_fps(i_fps)
{
}
frame_source_id frame_decoder_fps::get_id() const
{
	return m_id;
}
float frame_decoder_fps::get_fps() const
{
	return m_fps;
}
bool frame_decoder_fps::operator==(const frame_decoder_fps& other) const
{
	return m_fps == other.m_fps;
}
bool frame_decoder_fps::operator<(const frame_decoder_fps& other) const
{
	return m_id < other.m_id;
}

transform_time::transform_time(frame_source_id i_id,pfr::profiler_time_point i_reference)
: m_id(i_id)
, m_duration(std::chrono::duration_cast<std::chrono::milliseconds>(pfr::profiler_clock::now() - i_reference))
{
}
frame_source_id transform_time::get_id() const
{
	return m_id;
}
std::chrono::milliseconds transform_time::get_duration() const
{
	return m_duration;
}

}