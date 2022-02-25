#pragma once

#if defined(WIN32)

#pragma comment(lib, "ws2_32")

#endif

#include "IPlayerReceiver.h"
#include "PlayerReceiverDefs.h"
#include "PassiveSocket.h"
#include "ddk_thread_executor.h"
#include "ReceiverDefs.h"
#include "MCUConfigProvider.h"

namespace mcu
{

enum class TcpMessageType : uint8_t
{
	None,
	Init,
	URL,
	Position,
	Rotation,
	FOV,
	Disconnect
};

class TcpSocketPlayerReceiver : public IPlayerReceiver
{
public:
	TcpSocketPlayerReceiver(const MCUReceiverConfigProvider& i_configProvider);

private:
	void start_listen() override;
	bool listen_event(const std::string&,const ddk::function<bool(const MessageResponse&) >& i_callback) override;
	bool send_event(const IOrchestratorMessage&,const ddk::function<void(msg_result)>& i_ackCallback) override;
	void stop_listen() override;

	void onListen();

	CPassiveSocket m_socket;
	std::string m_ip;
	unsigned int m_port;
	ddk::thread_polling_executor m_executor;
};

}