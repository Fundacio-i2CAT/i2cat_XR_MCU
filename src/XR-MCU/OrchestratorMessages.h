#pragma once

#include "PlayerDefs.h"
#include "ddk_static_visitor.h"
#include "ddk_scoped_enum.h"
#include "ddk_optional.h"
#include <string>
#include <map>
#include <forward_list>
#include <memory>
#include <list>

namespace mcu
{

SCOPED_ENUM_DECL(MessageType,
				Connect,
				Login,
				NTPTime,
				Join,
				DeclareDataStream,
				InitPlayerInfo,
				RegisterDataStream,
				PlayerStateUpdate,
				NewFrame,
				FusionedFrame,
				Logout
);
SCOPED_ENUM_DECL_NAMES(MessageType,
					    "Connect",
						"LoginMCU",
					    "GetNTPTime",
					    "JoinSessionMCU",
						"DeclareDataStream",
						"InitialInfo",
						"RegisterForDataStream",
						"PlayerStateUpdate",
						"SendData",
						"SendData",
					    "Logout"
);

class MessageResponseEntry
{
public:
	typedef std::map<std::string,std::string>::iterator iterator;
	typedef std::map<std::string,std::string>::const_iterator const_iterator;

	MessageResponseEntry(const std::map<std::string,std::string>& i_contents);
	MessageResponseEntry(std::map<std::string,std::string>&& i_contents);

	bool has_key(const std::string& i_key) const;
	bool has_keys(const std::initializer_list<std::string>&) const;
	bool has_value(const std::string& i_value) const;
	std::string get_value(const std::string& i_key) const;
	std::string extract_value(const std::string& i_key);
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

private:
	std::map<std::string,std::string> m_keyValueMap;
};

class MessageResponse
{
	typedef std::list<MessageResponseEntry> entries_container;

public:
	typedef entries_container::iterator iterator;
	typedef entries_container::const_iterator const_iterator;

	MessageResponse(MessageType i_type,size_t i_msgId);
	MessageResponse(MessageType i_type, const std::string& i_name, const std::list<MessageResponseEntry>& i_contents);
	MessageResponse(MessageType i_type, const std::string& i_name,std::list<MessageResponseEntry>&& i_contents);
	MessageResponse(MessageType i_type,const std::string& i_name);
	MessageResponse(const MessageResponse& other);
	MessageResponse(MessageResponse&& other);

	MessageResponse& operator=(const MessageResponse& other);
	MessageResponse& operator=(MessageResponse&& other);

	void add_contents(const std::map<std::string,std::string>& i_contents);
	std::string get_name() const;
	MessageType get_type() const;
	size_t get_id() const;

	size_t size() const;
	bool empty() const;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;


private:
	std::string m_name;
	MessageType m_msgType;
	size_t m_msgId;
	std::list<MessageResponseEntry> m_entries;
};

struct Message
{
public:
	Message(const std::string& i_data, bool i_binary = false);

	std::string extract_contents();
	const char* get_data() const;
	size_t get_size() const;
	bool is_binary() const;

	template<typename T>
	inline const T* get_data_as() const
	{
		return reinterpret_cast<const T*>(m_data.data());
	}

private:
	const std::string m_data;
	const bool m_binary;
};

class IOrchestratorMessage
{
public:
	virtual ~IOrchestratorMessage() = default;

	virtual std::forward_list<Message> construct_message() const = 0;
	virtual MessageType get_type() const = 0;
	virtual size_t get_id() const = 0;
	virtual std::string get_name() const = 0;
	virtual ddk::optional<std::string> get_response_name() const = 0;
	virtual bool needs_ack() const = 0;
};

class OrchestratorMessage : public IOrchestratorMessage
{
public:
	OrchestratorMessage(MessageType i_type);

private:
	MessageType get_type() const final override;
	size_t get_id() const final override;
	std::string get_name() const override;
	ddk::optional<std::string> get_response_name() const override;
	bool needs_ack() const override;

	const MessageType m_type;
};

class OrchestratorLoginMessage : public OrchestratorMessage
{
public:
	OrchestratorLoginMessage(const std::string& i_userName, const std::string& i_pwd);

private:
	std::forward_list<Message> construct_message() const final override;

	const std::string m_userName;
	const std::string m_password;
};

class OrchestratorNTPTimeMessage : public OrchestratorMessage
{
public:
	OrchestratorNTPTimeMessage();

private:
	std::forward_list<Message> construct_message() const final override;
};

class OrchestratorJoinSessionMessage : public OrchestratorMessage
{
public:
	OrchestratorJoinSessionMessage(const std::string& i_sessionId, bool i_canBeMaster = true);

private:
	std::forward_list<Message> construct_message() const final override;

	const std::string m_sessionId;
	const bool m_canBeMaster;
};

class OrchestratorDeclareDataStreamMessage : public OrchestratorMessage
{
public:
	OrchestratorDeclareDataStreamMessage(const std::string& i_streamName);

private:
	std::forward_list<Message> construct_message() const final override;

	const std::string m_streamName;
};

class OrchestratorInitPlayerInfoMessage : public OrchestratorMessage
{
public:
	OrchestratorInitPlayerInfoMessage(const position_3d& i_pos, const rotation_2d& i_rot);

private:
	std::forward_list<Message> construct_message() const final override;
	ddk::optional<std::string> get_response_name() const final override;

	const position_3d m_pos;
	const rotation_2d m_rot;
};

class OrchestratorRegisterDataStreamMessage : public OrchestratorMessage
{
public:
	OrchestratorRegisterDataStreamMessage(player_id i_id, const std::string& i_streamName, const std::string& i_sessionId);

private:
	std::forward_list<Message> construct_message() const final override;

	const player_id m_id;
	const std::string m_streamName;
	const std::string m_sessionId;
};

class OrchestratorPlayerStateUpdateMessage : public OrchestratorMessage
{
public:
	OrchestratorPlayerStateUpdateMessage(const player_state_update& i_state);

private:

	struct visitor
	{
		typedef std::string return_type;
		struct callable_tag;

		std::string operator()(const position_3d& i_data) const;
		std::string operator()(const rotation_2d& i_data) const;
		std::string operator()(const fov_data& i_data) const;
		std::string operator()(const lod_data& i_data) const;
	};

	std::forward_list<Message> construct_message() const final override;

	player_state_update m_state;
};

class OrchestratorNewFrameMessage : public OrchestratorMessage
{
public:
	OrchestratorNewFrameMessage(const std::string& i_streamName, const std::shared_ptr<std::string>& i_data);

	std::shared_ptr<std::string> get_data();

private:
	std::forward_list<Message> construct_message() const final override;
	bool needs_ack() const override;

	const std::string m_streamName;
	std::shared_ptr<std::string> m_data;
};

class OrchestratorFusionedFrame : public OrchestratorMessage
{
public:
	OrchestratorFusionedFrame(const std::string& i_streamName, void* i_data, size_t i_size);

private:
	std::forward_list<Message> construct_message() const final override;

	const std::string m_streamName;
	void* m_data;
	size_t m_size;
};

class OrchestratorLogoutMessage : public OrchestratorMessage
{
public:
	OrchestratorLogoutMessage();

private:
	std::forward_list<Message> construct_message() const final override;
};

template<typename T, typename ... Args>
inline T make_message(Args&& ... i_args)
{
	return T{std::forward<Args>(i_args)...};
}

}