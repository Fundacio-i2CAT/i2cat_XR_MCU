#include "SocketIoPlayerReceiver.h"
#include "ReceiverDefs.h"
#include "SocketIoConnection.h"
#include "ddk_formatter.h"
#include "ddk_reference_wrapper.h"
#include <ctime>

namespace mcu
{

SocketIoPlayerReceiver::SocketIoPlayerReceiver(const MCUReceiverConfigProvider& i_configProvider)
{
	s_self = lend();
	const OrchestratorConfig orchConfig = i_configProvider.getOrchestratorConfig();
	const ConnectionConfig& connConfig = orchConfig.getConnectionConfig();

	m_url = connConfig.get_ip() + ":" + std::to_string(connConfig.get_port());

	m_connection = ddk::make_unique_reference<SocketIoConnection>(m_url);
	m_connection->sig_onStateChanged.connect(ddk::make_function(this,&SocketIoPlayerReceiver::onConnectionStateChanged));
	m_connection->sig_onNewResponse.connect(ddk::make_function(this,&SocketIoPlayerReceiver::onNewResponse));

	m_id = player_id("");

	pthread_mutex_init(&m_mutex,nullptr);
	pthread_cond_init(&m_condVar,nullptr);
}
SocketIoPlayerReceiver::~SocketIoPlayerReceiver()
{
	s_self = nullptr;
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_condVar);
}
void SocketIoPlayerReceiver::start_listen()
{
	if(connect())
	{
		m_executor.start_thread(ddk::make_function(this,&SocketIoPlayerReceiver::update));
	}
}
bool SocketIoPlayerReceiver::listen_event(const std::string& i_eventName,const ddk::function<bool(const MessageResponse&)>& i_callback)
{
	if(m_connection)
	{
		return m_connection->listen(i_eventName,i_callback);
	}
	else
	{
		return false;
	}
}
bool SocketIoPlayerReceiver::send_event(const IOrchestratorMessage& i_event,const ddk::function<void(msg_result)>& i_ackCallback)
{
	bool res = false;

	pthread_mutex_lock(&m_mutex);

	if(m_connection)
	{
		res = m_connection->send(i_event,i_ackCallback);
	}

	pthread_mutex_unlock(&m_mutex);

	return res;
}
bool SocketIoPlayerReceiver::register_stream(const std::string& i_streamName)
{
	if(m_connection)
	{
		m_connection->send(make_message<OrchestratorDeclareDataStreamMessage>(i_streamName),ddk::make_function(this,&SocketIoPlayerReceiver::onAck));

		return waitForAck();
	}
	else
	{
		return false;
	}
}
void SocketIoPlayerReceiver::stop_listen()
{
	m_connection->stop();

	m_state = GetOut;
}
bool SocketIoPlayerReceiver::is_listening() const
{
	return m_state == InRoom;
}
bool SocketIoPlayerReceiver::connect()
{
	m_state = Connecting;

	m_connection->start();

	pthread_mutex_lock(&m_mutex);

	bool ackOkReceived = waitForAck();

	pthread_mutex_unlock(&m_mutex);

	return ackOkReceived;
}
void SocketIoPlayerReceiver::update()
{
	//handshake
	while(goToNextState());

	//session loop
	while(m_state != GetOut)
	{
		pthread_mutex_lock(&m_mutex);

		m_receivedAck = none;

		waitForAck();

		pthread_mutex_unlock(&m_mutex);

		goToNextState();
	}
}
void SocketIoPlayerReceiver::onConnectionStateChanged(IClientConnection::State i_oldState,IClientConnection::State i_newState)
{
	if(i_newState == IClientConnection::Connected && m_state == Connecting)
	{
		m_state = Connected;

		onAck(MessageResponse(MessageType::Connect,0));

		sig_onSocketChanged.execute(ddk::lend(m_connection));
	}
	else if(i_newState == IClientConnection::Idle && m_state != Connecting)
	{
		m_state = Idle;

		onAck(MessageResponse(MessageType::Connect,0));
	}
}
void SocketIoPlayerReceiver::onNewResponse(const MessageResponse& i_response)
{
	pthread_mutex_lock(&m_mutex);

	m_response = i_response;

	pthread_cond_signal(&m_condVar);

	pthread_mutex_unlock(&m_mutex);
}
bool SocketIoPlayerReceiver::onNewPlayer(const MessageResponse& i_response)
{
	pthread_mutex_lock(&m_mutex);

	MessageResponse::const_iterator itMsg = i_response.begin();
	for(; itMsg != i_response.end(); ++itMsg)
	{
		if(m_sessionId.empty() && itMsg->has_key("sessionId"))
		{
			m_sessionId = itMsg->get_value("sessionId");
		}
		if(itMsg->has_keys({ "id","url","posx","posy","posz","rot" }))
		{
			PlayerInfo info;

			info.id = itMsg->get_value("id");
			info.streamName = itMsg->get_value("url");
			info.position[0] = stoi(itMsg->get_value("posx"));
			info.position[1] = stoi(itMsg->get_value("posy"));
			info.position[2] = stoi(itMsg->get_value("posz"));

			info.rotation = stoi(itMsg->get_value("rot"));

			m_registeredPlayers.push_back(info);
		}
	}

	m_response = i_response;

	pthread_cond_signal(&m_condVar);

	pthread_mutex_unlock(&m_mutex);

	return true;
}
void SocketIoPlayerReceiver::send(const IOrchestratorMessage& i_msg)
{
	MessageResponse response(i_msg.get_type(),i_msg.get_type());

	m_response = none;
	if(m_receivedAck)
	{
		m_receivedAck->dismiss();
	}
	m_receivedAck = none;

	if(m_connection->send(i_msg,ddk::make_function(this,&SocketIoPlayerReceiver::onAck)))
	{
		if(i_msg.needs_ack())
		{
			if(waitForAck())
			{
				response = m_receivedAck->get();
			}
		}

		if(const ddk::optional<std::string> responseName = i_msg.get_response_name())
		{
			const ddk::optional<MessageResponse> responseOpt = waitForResponse();

			if(responseOpt)
			{
				response = *responseOpt;
			}
			else
			{
				m_state = GetOut;
			}
		}

		processResponse(response);
	}
	else
	{
		m_state = GetOut;
	}
}
bool SocketIoPlayerReceiver::waitForAck()
{
	if(m_receivedAck.empty())
	{
		pthread_cond_wait(&m_condVar,&m_mutex);
	}

	return m_receivedAck && static_cast<bool>(*m_receivedAck);
}
ddk::optional<MessageResponse> SocketIoPlayerReceiver::waitForResponse()
{
	while(m_response.empty())
	{
		pthread_cond_wait(&m_condVar,&m_mutex);
	}

	return m_response;
}
void SocketIoPlayerReceiver::onAck(msg_result i_error) const
{
	pthread_mutex_lock(&m_mutex);

	m_receivedAck = std::move(i_error);

	pthread_cond_signal(&m_condVar);

	pthread_mutex_unlock(&m_mutex);
}
void SocketIoPlayerReceiver::processResponse(const MessageResponse& i_response)
{
	if(m_state == Logging)
	{
		if(i_response.size() == 1)
		{
			MessageResponse::const_iterator itMsg = i_response.begin();
			if(itMsg->has_key("userId"))
			{
				m_userId = itMsg->get_value("userId");

				m_state = Logged;
			}
		}
		else
		{
			m_state = GetOut;
		}
	}
	else if(m_state == Logged)
	{
		if(i_response.size() == 1)
		{
			MessageResponse::const_iterator itMsg = i_response.begin();
			if(itMsg->has_key("ntpTimeMs"))
			{
				const time_t ntpTimeInMs = std::stoll(itMsg->get_value("ntpTimeMs"));
				const time_t ntpTimeInS = ntpTimeInMs / 1000;

				const std::tm* ntpDate = gmtime(&ntpTimeInS);

				mcu::mcu_frame_clock::calibrate(mcu_frame_clock::from_time_t(ntpTimeInS) + std::chrono::milliseconds(ntpTimeInMs - ntpTimeInS * 1000));
			}
		}
		else
		{
			DDK_LOG_ERROR("NTP time was not received form orchestrator.");
		}
	}
	else if(m_state == DeclaringdDataStream)
	{
		m_state = InRoom;

		sig_onSessionStablished.execute();
	}
	else if(m_state == WaitingPlayerInfo || m_state == SendingPlayerInfo)
	{
		//leave some time for the bunch of users to be registeres
		std::this_thread::sleep_for(std::chrono::seconds(3));

		m_state = ReceivedPlayers;
	}
	else if(m_state == JoiningSession)
	{
		m_state = JoinedSession;
	}
}
bool SocketIoPlayerReceiver::goToNextState()
{
	bool keepGoing = true;

	pthread_mutex_lock(&m_mutex);

	//intermediate states
	if(m_state == Idle)
	{
		m_state = Connecting;

		m_connection->start();

		waitForAck();
	}
	else if(m_state == Connected)
	{
		m_state = Logging;

		send(make_message<OrchestratorLoginMessage>("12333333","1"));
	}
	else if(m_state == Logged)
	{
		//once logged, ask for its ntp time
		send(make_message<OrchestratorNTPTimeMessage>());

		m_state = WaitingPlayerInfo;

		m_response = none;
		m_receivedAck = none;

		m_connection->listen("InitPlayer",ddk::make_function(this,&SocketIoPlayerReceiver::onNewPlayer));

		if(ddk::optional<MessageResponse> optResponse = waitForResponse())
		{
			processResponse(*optResponse);
		}
		else
		{
			keepGoing = false;
		}
	}
	else if(m_state == ReceivedPlayers)
	{
		m_state = JoiningSession;

		send(make_message<OrchestratorJoinSessionMessage>(m_sessionId));
	}
	else if(m_state == JoinedSession)
	{
		m_state = DeclaringdDataStream;

		send(make_message<OrchestratorDeclareDataStreamMessage>(m_userId + "mcu#0"));
	}
	else if(m_state == InRoom)
	{
		const std::vector<PlayerInfo> localRegisteredPlayers = std::move(m_registeredPlayers);
		std::vector<PlayerInfo>::const_iterator itPlayer = localRegisteredPlayers.begin();
		for(; itPlayer != localRegisteredPlayers.end(); ++itPlayer)
		{
			send(make_message<OrchestratorRegisterDataStreamMessage>(player_id(itPlayer->id),itPlayer->streamName,m_sessionId));

			//after ack
			if(m_receivedAck)
			{
				if(const msg_result& ack = *m_receivedAck)
				{
					informAboutNewUser(*itPlayer);
				}
			}
		}

		keepGoing = false;
	}
	else
	{
		keepGoing = false;
	}

	pthread_mutex_unlock(&m_mutex);

	return keepGoing;
}
void SocketIoPlayerReceiver::informAboutNewUser(const PlayerInfo& i_info)
{
	DDK_LOG_INFO("New Player");

	send(make_message<OrchestratorRegisterDataStreamMessage>(player_id(i_info.id),i_info.streamName,m_sessionId));

	//after ack
	if(m_receivedAck)
	{
		if(const msg_result& ack = *m_receivedAck)
		{
			NetMessage msg;

			msg.id = i_info.id;
			msg.type = static_cast<uint8_t>(FrameReaderType::Socketio);
			msg.url = i_info.streamName;
			msg.position[0] = i_info.position[0];
			msg.position[1] = i_info.position[1];
			msg.position[2] = i_info.position[2];
			msg.rotation = i_info.rotation;

			const PlayerCreateMessage playerMsg(msg);

			sig_onPlayerCreated.execute(player_id(msg.id),playerMsg);
		}
	}


	DDK_LOG_INFO("New Player DONE");
}

}
