#include "OrchestratorMessages.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "ddk_none.h"

namespace mcu
{
namespace detail
{

size_t __get_next_message_id()
{
	static size_t nextId = 0;

	return nextId++;
}

}

Message::Message(const std::string& i_data,bool i_binary)
: m_data(i_data)
, m_binary(i_binary)
{
}
std::string Message::extract_contents()
{
	return std::move(m_data);
}
const char* Message::get_data() const
{
	return m_data.data();
}
size_t Message::get_size() const
{
	return m_data.size();
}
bool Message::is_binary() const
{
	return m_binary;
}

OrchestratorMessage::OrchestratorMessage(MessageType i_type)
: m_type(i_type)
{
}
MessageType OrchestratorMessage::get_type() const
{
	return m_type;
}
size_t OrchestratorMessage::get_id() const
{
	return detail::__get_next_message_id();
}
std::string OrchestratorMessage::get_name() const
{
	return get_enum_name(m_type);
}
ddk::optional<std::string> OrchestratorMessage::get_response_name() const
{
	return none;
}
bool OrchestratorMessage::needs_ack() const
{
	return true;
}

MessageResponseEntry::MessageResponseEntry(const std::map<std::string,std::string>& i_contents)
: m_keyValueMap(i_contents)
{
}
MessageResponseEntry::MessageResponseEntry(std::map<std::string,std::string>&& i_contents)
: m_keyValueMap(std::move(i_contents))
{
}
bool MessageResponseEntry::has_key(const std::string& i_key) const
{
	return m_keyValueMap.find(i_key) != m_keyValueMap.end();
}
bool MessageResponseEntry::has_keys(const std::initializer_list<std::string>& i_elems) const
{
	for(const auto& elem : i_elems)
	{
		if(m_keyValueMap.find(elem) == m_keyValueMap.end())
		{
			return false;
		}
	}

	return true;
}
bool MessageResponseEntry::has_value(const std::string& i_value) const
{
	if(m_keyValueMap.empty() == false)
	{
		for(const auto& elem : m_keyValueMap)
		{
			if(elem.second == i_value)
			{
				return true;
			}
		}
	}

	return false;
}
std::string MessageResponseEntry::get_value(const std::string& i_key) const
{
	return m_keyValueMap.find(i_key)->second;
}
std::string MessageResponseEntry::extract_value(const std::string& i_key)
{
	std::map<std::string,std::string>::iterator itValue = m_keyValueMap.find(i_key);

	std::string res = std::move(itValue->second);

	m_keyValueMap.erase(itValue);

	return std::move(res);
}
MessageResponseEntry::iterator MessageResponseEntry::begin()
{
	return m_keyValueMap.begin();
}
MessageResponseEntry::const_iterator MessageResponseEntry::begin() const
{
	return m_keyValueMap.begin();
}
MessageResponseEntry::iterator MessageResponseEntry::end()
{
	return m_keyValueMap.end();
}
MessageResponseEntry::const_iterator MessageResponseEntry::end() const
{
	return m_keyValueMap.end();
}

MessageResponse::MessageResponse(MessageType i_type,size_t i_msgId)
: m_msgType(i_type)
, m_msgId(i_msgId)
{
}
MessageResponse::MessageResponse(MessageType i_type, const std::string& i_name,const std::list<MessageResponseEntry>& i_contents)
: m_msgType(i_type)
, m_name(i_name)
, m_entries(i_contents)
{
}
MessageResponse::MessageResponse(MessageType i_type, const std::string& i_name,std::list<MessageResponseEntry>&& i_contents)
: m_msgType(i_type)
, m_name(i_name)
, m_entries(std::move(i_contents))
{
}
MessageResponse::MessageResponse(MessageType i_type,const std::string& i_name)
: m_msgType(i_type)
, m_name(i_name)
{
}
MessageResponse::MessageResponse(const MessageResponse& other)
: m_name(other.m_name)
, m_msgType(other.m_msgType)
, m_msgId(other.m_msgId)
, m_entries(other.m_entries)
{
}
MessageResponse::MessageResponse(MessageResponse&& other)
: m_name(std::move(other.m_name))
,m_msgType(std::move(other.m_msgType))
,m_msgId(std::move(other.m_msgId))
,m_entries(std::move(other.m_entries))
{
}
MessageResponse& MessageResponse::operator=(const MessageResponse& other)
{
	m_name = other.m_name;
	m_msgType = other.m_msgType;
	m_msgId = other.m_msgId;
	m_entries = other.m_entries;

	return *this;
}
MessageResponse& MessageResponse::operator=(MessageResponse&& other)
{
	m_name = std::move(other.m_name);
	m_msgType = std::move(other.m_msgType);
	m_msgId = std::move(other.m_msgId);
	m_entries = std::move(other.m_entries);

	return *this;
}
void MessageResponse::add_contents(const std::map<std::string,std::string>& i_contents)
{
	m_entries.emplace_back(i_contents);
}
std::string MessageResponse::get_name() const
{
	return m_name;
}
MessageType MessageResponse::get_type() const
{
	return m_msgType;
}
size_t MessageResponse::get_id() const
{
	return m_msgId;
}
size_t MessageResponse::size() const
{
	return m_entries.size();
}
bool MessageResponse::empty() const
{
	return m_entries.empty();
}
MessageResponse::iterator MessageResponse::begin()
{
	return m_entries.begin();
}
MessageResponse::iterator MessageResponse::end()
{
	return m_entries.end();
}
MessageResponse::const_iterator MessageResponse::begin() const
{
	return m_entries.begin();
}
MessageResponse::const_iterator MessageResponse::end() const
{
	return m_entries.end();
}

OrchestratorLoginMessage::OrchestratorLoginMessage(const std::string& i_userName,const std::string& i_pwd)
: OrchestratorMessage(MessageType::Login)
, m_userName(i_userName)
, m_password(i_pwd)
{
}
std::forward_list<Message> OrchestratorLoginMessage::construct_message() const
{
	return { m_userName, m_password };
}

OrchestratorNTPTimeMessage::OrchestratorNTPTimeMessage()
: OrchestratorMessage(MessageType::NTPTime)
{
}
std::forward_list<Message> OrchestratorNTPTimeMessage::construct_message() const
{
	return {std::string("")};
}

OrchestratorJoinSessionMessage::OrchestratorJoinSessionMessage(const std::string& i_sessionId,bool i_canBeMaster)
: OrchestratorMessage(MessageType::Join)
, m_sessionId(i_sessionId)
, m_canBeMaster(i_canBeMaster)
{
}
std::forward_list<Message> OrchestratorJoinSessionMessage::construct_message() const
{
	return { m_sessionId };
}

OrchestratorDeclareDataStreamMessage::OrchestratorDeclareDataStreamMessage(const std::string& i_streamName)
: OrchestratorMessage(MessageType::DeclareDataStream)
, m_streamName(i_streamName)
{
}
std::forward_list<Message> OrchestratorDeclareDataStreamMessage::construct_message() const
{
	return { m_streamName, std::string("Empty description") };
}

OrchestratorInitPlayerInfoMessage::OrchestratorInitPlayerInfoMessage(const position_3d& i_pos,const rotation_2d& i_rot)
: OrchestratorMessage(MessageType::InitPlayerInfo)
, m_pos(i_pos)
, m_rot(i_rot)
{
}
std::forward_list<Message> OrchestratorInitPlayerInfoMessage::construct_message() const
{
	std::string initialPlayerInfoStr;

	rapidjson::Document d;
	d.SetObject();

	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();


	rapidjson::Value initialPlayerInfoVal(rapidjson::kObjectType);

	rapidjson::Value posObj(rapidjson::kObjectType);
	rapidjson::Value posVal(rapidjson::kObjectType);

	rapidjson::Value posComponentObj(rapidjson::kObjectType);
	rapidjson::Value posComponentVal(rapidjson::kObjectType);

	posComponentVal.SetDouble(m_pos[0]);
	posComponentObj.AddMember("x",posComponentVal,allocator);
	posComponentVal.SetDouble(m_pos[1]);
	posComponentObj.AddMember("y",posComponentVal,allocator);
	posComponentVal.SetDouble(m_pos[2]);
	posComponentObj.AddMember("z",posComponentVal,allocator);

	initialPlayerInfoVal.AddMember("pos",posComponentObj,allocator);

	rapidjson::Value rotValue;

	rotValue.SetDouble(m_rot);

	initialPlayerInfoVal.AddMember("rot",rotValue,allocator);

	rapidjson::Value commandIdValue(rapidjson::kObjectType);
	commandIdValue.SetUint(17);

	d.AddMember("initialPlayerInfoJson",initialPlayerInfoVal,allocator);
	d.AddMember("commandId",commandIdValue,allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	d.Accept(writer);
	initialPlayerInfoStr = strbuf.GetString();

	const std::string jsonTxt = strbuf.GetString();

	return { jsonTxt };
}
ddk::optional<std::string> OrchestratorInitPlayerInfoMessage::get_response_name() const
{
	return "InitPlayer";
}

OrchestratorRegisterDataStreamMessage::OrchestratorRegisterDataStreamMessage(player_id i_id,const std::string& i_streamName,const std::string& i_sessionId)
: OrchestratorMessage(MessageType::RegisterDataStream)
, m_id(i_id)
, m_streamName(i_streamName)
, m_sessionId(i_sessionId)
{
}
std::forward_list<Message> OrchestratorRegisterDataStreamMessage::construct_message() const
{
	return { m_id.getValue(), m_streamName };
}

OrchestratorPlayerStateUpdateMessage::OrchestratorPlayerStateUpdateMessage(const player_state_update& i_state)
: OrchestratorMessage(MessageType::PlayerStateUpdate)
, m_state(i_state)
{
}
std::forward_list<Message> OrchestratorPlayerStateUpdateMessage::construct_message() const
{
	return { ddk::visit<visitor>(m_state) };
}

std::string OrchestratorPlayerStateUpdateMessage::visitor::operator()(const position_3d& i_data) const
{
	return "";
}
std::string OrchestratorPlayerStateUpdateMessage::visitor::operator()(const rotation_2d& i_data) const
{
	return "";
}
std::string OrchestratorPlayerStateUpdateMessage::visitor::operator()(const fov_data& i_data) const
{
	return "";
}
std::string OrchestratorPlayerStateUpdateMessage::visitor::operator()(const lod_data& i_data) const
{
	return "";
}

OrchestratorNewFrameMessage::OrchestratorNewFrameMessage(const std::string& i_streamName, const std::shared_ptr<std::string>& i_data)
: OrchestratorMessage(MessageType::NewFrame)
, m_streamName(i_streamName)
, m_data(i_data)
{
}
std::shared_ptr<std::string> OrchestratorNewFrameMessage::get_data()
{
	return std::move(m_data);
}
std::forward_list<Message> OrchestratorNewFrameMessage::construct_message() const
{
	return { m_streamName, std::string("prova") };
}
bool OrchestratorNewFrameMessage::needs_ack() const
{
	return false;
}

OrchestratorFusionedFrame::OrchestratorFusionedFrame(const std::string& i_streamName,void* i_data,size_t i_size)
: OrchestratorMessage(MessageType::FusionedFrame)
, m_streamName(i_streamName)
, m_data(i_data)
, m_size(i_size)
{
}
std::forward_list<Message> OrchestratorFusionedFrame::construct_message() const
{
	return { m_streamName, {std::string(reinterpret_cast<char*>(m_data),m_size),true} };
}

OrchestratorLogoutMessage::OrchestratorLogoutMessage()
: OrchestratorMessage(MessageType::Logout)
{
}
std::forward_list<Message> OrchestratorLogoutMessage::construct_message() const
{
	return { std::string() };
}

}