#include "TcpSocketPlayerReceiver.h"
#include "ddk_formatter.h"
#include "CPacket.h"

namespace mcu
{

const std::chrono::milliseconds k_pollingTime = std::chrono::milliseconds(40);

TcpSocketPlayerReceiver::TcpSocketPlayerReceiver(const MCUReceiverConfigProvider& i_configProvider)
{
	const OrchestratorConfig orchConfig = i_configProvider.getOrchestratorConfig();
	const ConnectionConfig& connConfig = orchConfig.getConnectionConfig();

	m_ip = connConfig.get_ip();
	m_port = connConfig.get_port();
	m_executor.set_update_time(k_pollingTime);
	m_socket.Initialize();
}
void TcpSocketPlayerReceiver::start_listen()
{
	m_socket.Listen(m_ip.c_str(), m_port);

	DDK_LOG_INFO("Start listening");

	m_executor.start_thread(ddk::make_function(this,&TcpSocketPlayerReceiver::onListen));
}
bool TcpSocketPlayerReceiver::listen_event(const std::string&,const ddk::function<bool(const MessageResponse&) >& i_callback)
{
	DDK_FAIL("Operation not supported")

	return false;
}
bool TcpSocketPlayerReceiver::send_event(const IOrchestratorMessage&,const ddk::function<void(msg_result)>& i_ackCallback)
{
	DDK_FAIL("Operation not supported")

		return false;
}
void TcpSocketPlayerReceiver::stop_listen()
{
	m_executor.stop_thread();
}
//Modificar los métodos para las funciones adaptados a socket.io
void TcpSocketPlayerReceiver::onListen()
{
	if (CActiveSocket *pClient = m_socket.Accept())
    {
        if (pClient->Receive(MAX_PACKET))
        {
			NetMessage msg; 


			CPacket packet(pClient->GetData(),pClient->GetBytesReceived());

			int packetSize = pClient->GetBytesReceived();

			packet >> msg.id >> msg.type;

			DDK_LOG_INFO("Message Received: " << packetSize << " Bytes");
			DDK_LOG_INFO("ID: " << msg.id);
			DDK_LOG_INFO("Type: " << (int)msg.type);

			switch (TcpMessageType(msg.type))
			{
				case TcpMessageType::Init:
				{
					packet >> msg.position[0] >> msg.position[1] >> msg.position[2] >> msg.rotation;

					DDK_LOG_INFO("Position: " << msg.position[0] << ", " << msg.position[1] << ", " << msg.position[2]);
					DDK_LOG_INFO("Rotation: " << msg.rotation);
					int stringLength = packetSize - packet.get_size();
					DDK_LOG_INFO("String length: " << stringLength << " Bytes");

					for (int i = 0; i < stringLength; ++i) {
						uint8_t letter;
						packet >> letter;
						msg.url += letter;
					}

					DDK_LOG_INFO("URL: " << msg.url);

					const PlayerCreateMessage playerMsg(msg);

					sig_onPlayerCreated.execute(player_id(msg.id),playerMsg);

					break;
				}
				case TcpMessageType::URL:
				{
					packet >> msg.url;

					const PlayerUpdateMessage playerMsg(msg);

					sig_onPlayerUpdated.execute(player_id(msg.id),playerMsg);

					break;
				}
				case TcpMessageType::Position:
				{
					packet >> msg.position[0] >> msg.position[1] >> msg.position[2];

					const PlayerUpdateMessage playerMsg(msg);

					sig_onPlayerUpdated.execute(player_id(msg.id),playerMsg);

					break;
				}
				case TcpMessageType::Rotation:
				{
					packet >> msg.rotation;

					const PlayerUpdateMessage playerMsg(msg);

					sig_onPlayerUpdated.execute(player_id(msg.id),playerMsg);

					break;
				}
				case TcpMessageType::FOV:
				{
					packet >> msg.fov;

					const PlayerUpdateMessage playerMsg(msg);

					sig_onPlayerUpdated.execute(player_id(msg.id),playerMsg);

					break;
				}
				case TcpMessageType::Disconnect:
				{
					sig_onPlayerDeleted.execute(player_id(msg.id));

					break;
				}
			}

            pClient->Close();
        }

        delete pClient;
    }
}

}