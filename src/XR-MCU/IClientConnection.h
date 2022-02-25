#pragma once

#include "OrchestratorMessages.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_signal.h"
#include "ddk_type_id.h"
#include "ddk_result.h"

namespace mcu
{

struct msg_error_t;
typedef ddk::Id<size_t,msg_error_t> msg_error;
typedef ddk::result<MessageResponse,msg_error> msg_result;

class IClientConnection
{
public:
	enum State
	{
		Idle,
		Connecting,
		Connected
	};

	~IClientConnection() = default;

	virtual void start() = 0;
	virtual bool send(const IOrchestratorMessage&, const ddk::function<void(msg_result)>& i_ackCallback = nullptr) = 0;
	virtual bool listen(const std::string&, const ddk::function<bool(const MessageResponse&)>&) = 0;
	virtual void stop() = 0;
	virtual bool is_connected() const = 0;

	ddk::signal<void(State,State)> sig_onStateChanged;
	ddk::signal<void(const MessageResponse&)> sig_onNewResponse;
};

typedef ddk::unique_reference_wrapper<IClientConnection> client_connection_unique_ref;
typedef ddk::unique_reference_wrapper<const IClientConnection> client_connection_const_unique_ref;
typedef ddk::unique_pointer_wrapper<IClientConnection> client_connection_unique_ptr;
typedef ddk::unique_pointer_wrapper<const IClientConnection> client_connection_const_unique_ptr;

typedef ddk::lent_reference_wrapper<IClientConnection> client_connection_lent_ref;
typedef ddk::lent_reference_wrapper<const IClientConnection> client_connection_const_lent_ref;
typedef ddk::lent_pointer_wrapper<IClientConnection> client_connection_lent_ptr;
typedef ddk::lent_pointer_wrapper<const IClientConnection> client_connection_const_lent_ptr;

}