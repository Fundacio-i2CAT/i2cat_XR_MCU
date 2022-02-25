#pragma once

#include "FrameDecoderDefs.h"
#include "FrameEncoderDefs.h"
#include "FrameReaderDefs.h"
#include "PlayerUtils.h"
#include "ddk_type_id.h"
#include "ddk_variant.h"
#include <exception>

namespace mcu
{

struct NetMessage;

struct player_id_t;
typedef ddk::Id<std::string,player_id_t> player_id;
typedef std::array<float,2> fov_data;
typedef std::array<int,2> lod_data;

typedef ddk::variant<position_3d,rotation_2d,fov_data,lod_data> player_state_update;

struct PlayerData
{
	std::array<float,3> m_pos = { 0 };
	float m_rotation = 0.f;
	std::string m_url;
};

struct PlayerCreateMessage
{
public:
	PlayerCreateMessage(const NetMessage& i_msg);

	PlayerData get_data() const;
	FrameReaderType get_reader_type() const;
	DecodecType get_decoder_type() const;
	EncodecType get_encoder_type() const;
	FrameWriterType get_writer_type() const;

private:
	FrameReaderType m_readerType = FrameReaderType::Sub;
	DecodecType m_decoderType = DecodecType::Cwipc;
	EncodecType m_encoderType = EncodecType::Cwipc;
	FrameWriterType m_writerType = FrameWriterType::B2D;
	//FrameWriterType m_writerType = FrameWriterType::socket;
	PlayerData m_data;
};

struct PlayerUpdateMessage
{
	typedef ddk::variant<std::array<float,3>,float,Frustrum,std::string> msg_t;

public:
	PlayerUpdateMessage(const NetMessage& i_msg);

	template<typename T>
	bool is() const
	{
		return m_msg.is<T>();
	}
	template<typename T>
	const T& get() const
	{
		return m_msg.get<T>();
	}
	template<typename Visitor>
	typename Visitor::return_type visit(const Visitor& i_visitor) const
	{
		return m_msg.visit(i_visitor);
	}
	template<typename Visitor>
	typename Visitor::return_type visit(Visitor& i_visitor) const
	{
		return m_msg.visit(i_visitor);
	}

private:
	msg_t m_msg;
};

struct PlayerCreationException : public std::exception
{
};

}