#pragma once

#include "FrameDefs.h"
#include "ProfilerDefs.h"

namespace mcu
{

enum class DecodecType
{
	Cwipc
};

struct decoder_context
{
	DecodecType m_type;
};

struct pointcloud_size
{
public:
	pointcloud_size(frame_source_id i_id, size_t i_size);

	frame_source_id get_id() const;
	size_t get_size() const;

private:
	const frame_source_id m_id;
	const size_t m_size;
};

struct frame_receiver_fps
{
public:
	frame_receiver_fps(const std::string& i_id, float i_fps = 0);

	std::string get_id() const;
	float get_fps() const;

	bool operator==(const frame_receiver_fps& other) const;
	bool operator<(const frame_receiver_fps& other) const;

private:
	const std::string m_id;
	const float m_fps;
};

struct frame_latency
{
public:
	frame_latency(frame_source_id i_id, mcu_frame_time_point i_reference);

	frame_source_id get_id() const;
	std::chrono::milliseconds get_latency() const;

private:
	const frame_source_id m_id;
	const std::chrono::milliseconds m_latency;
};

struct decoding_time
{
public:
	decoding_time(frame_source_id i_id, pfr::profiler_time_point i_reference);

	frame_source_id get_id() const;
	std::chrono::milliseconds get_duration() const;

private:
	const frame_source_id m_id;
	const std::chrono::milliseconds m_duration;
};

struct frame_decoder_fps
{
public:
	frame_decoder_fps(frame_source_id i_id, float i_fps = 0);

	frame_source_id get_id() const;
	float get_fps() const;

	bool operator==(const frame_decoder_fps& other) const;
	bool operator<(const frame_decoder_fps& other) const;

private:
	const frame_source_id m_id;
	const float m_fps;
};

struct transform_time
{
public:
	transform_time(frame_source_id i_id,pfr::profiler_time_point i_reference);

	frame_source_id get_id() const;
	std::chrono::milliseconds get_duration() const;

private:
	const frame_source_id m_id;
	const std::chrono::milliseconds m_duration;
};

}