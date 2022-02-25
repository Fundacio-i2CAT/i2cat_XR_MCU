#include "PlayerDefs.h"
#include "ReceiverDefs.h"
#include "PlayerUtils.h"

namespace mcu
{

PlayerCreateMessage::PlayerCreateMessage(const NetMessage& i_msg)
{
	m_data.m_rotation = i_msg.rotation;
	m_data.m_url = i_msg.url;
	m_data.m_pos[0] = i_msg.position[0];
	m_data.m_pos[1] = i_msg.position[1];
	m_data.m_pos[2] = i_msg.position[2];
	m_readerType = FrameReaderType(i_msg.type);
	m_writerType = FrameWriterType(i_msg.type);
}
PlayerData PlayerCreateMessage::get_data() const
{
	PlayerData playerData;

	playerData.m_pos = m_data.m_pos;
	playerData.m_rotation = m_data.m_rotation;
	playerData.m_url = m_data.m_url;

	return playerData;
}
FrameReaderType PlayerCreateMessage::get_reader_type() const
{
	return m_readerType;
}
DecodecType PlayerCreateMessage::get_decoder_type() const
{
	return m_decoderType;
}
EncodecType PlayerCreateMessage::get_encoder_type() const
{
	return m_encoderType;
}
FrameWriterType PlayerCreateMessage::get_writer_type() const
{
	return m_writerType;
}

PlayerUpdateMessage::PlayerUpdateMessage(const NetMessage& i_msg)
{
	//switch(i_msg.type)
	//{
	//	case MessageType::Position:
	//	{
	//		m_msg = std::array<float,3>({ i_msg.position[0],i_msg.position[1],i_msg.position[2] });

	//		break;
	//	}
	//	case MessageType::Rotation:
	//	{
	//		m_msg = i_msg.rotation;

	//		break;
	//	}
	//	case MessageType::FOV:
	//	{
	//		m_msg = i_msg.fov;

	//		break;
	//	}
	//	case MessageType::URL:
	//	{
	//		m_msg = i_msg.url;

	//		break;
	//	}
	//}
}

}