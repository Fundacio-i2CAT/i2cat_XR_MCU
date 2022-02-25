#pragma once

#include "FrameEncoderExceptions.h"
#include "FrameDefs.h"
#include "FusionDefs.h"
#include "ProfilerDefs.h"
#include "ddk_type_id.h"

namespace mcu
{

enum class EncodecType
{
	Cwipc
};

enum class FrameWriterType
{
	B2D,
	SockettIo
};

struct encoder_context
{
	EncodecType m_codec;
	FrameWriterType m_sendSystem;
	std::string exchangeName;
	std::string publishUrl;
	size_t m_numThreadsPerEncoder;
	size_t m_maxPendingDecodedFrames;
};

struct frame_dest_id_t;
typedef ddk::Id<std::string,frame_dest_id_t> frame_dest_id;

struct encoding_time
{
public:
	encoding_time(frame_dest_id i_id,pfr::profiler_time_point i_reference);
	encoding_time(frame_dest_id i_id,mcu_frame_time_point i_reference);

	frame_dest_id get_id() const;
	std::chrono::milliseconds get_duration() const;

private:
	const frame_dest_id m_id;
	const std::chrono::milliseconds m_duration;
};

struct frame_fusioned_fps
{
public:
	frame_fusioned_fps(frame_dest_id  i_id, float i_fps = 0);

	frame_dest_id get_id() const;
	float get_fps() const;

private:
	const frame_dest_id m_id;
	const float m_fps;
};

struct frame_sent_fps
{
public:
	frame_sent_fps(frame_dest_id  i_id, float i_fps = 0);

	frame_dest_id get_id() const;
	float get_fps() const;

private:
	const frame_dest_id m_id;
	const float m_fps;
};

}