#pragma once

#include "IClientConnection.h"
#include "sio_client.h"

namespace sio
{

class client;

}

namespace mcu
{

class SocketIoConnection : public IClientConnection
{
public:
	SocketIoConnection(const std::string& i_url);
	~SocketIoConnection();

private:
	void start() final override;
	void stop() final override;
	bool is_connected() const final override;
	bool send(const IOrchestratorMessage& i_state,const ddk::function<void(msg_result)>& i_ackCallback) final override;
	bool listen(const std::string&,const ddk::function<bool(const MessageResponse&)>&) override;

	const std::string m_url;
	sio::client m_client;
	sio::socket::ptr m_socket;
	State m_state = Idle;
	std::map<std::string,std::list<ddk::function<bool(const MessageResponse&)>>> m_eventBindings;
};

}