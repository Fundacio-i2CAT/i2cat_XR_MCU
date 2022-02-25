#pragma once

#include "MCUConfigProvider.h"
#include "IPlayerReceiver.h"
#include "IClientConnection.h"
#include "sio_client.h"
#include "ddk_thread_executor.h"
#include "ddk_lend_from_this.h"

namespace mcu
{

class SocketIoPlayerReceiver: public IPlayerReceiver,public ddk::lend_from_this<SocketIoPlayerReceiver,IPlayerReceiver>
{
	friend class SocketIoFrameReader;

public:
	SocketIoPlayerReceiver(const MCUReceiverConfigProvider& i_configProvider);
	~SocketIoPlayerReceiver();

private:
	enum State
	{
		Idle,
		Connecting,
		Connected,
		Logging,
		Logged,
		SendingPlayerInfo,
		WaitingPlayerInfo,
		ReceivedPlayers,
		JoiningSession,
		JoinedSession,
		DeclaringdDataStream,
		DeclaredDataStream,
		InRoom,
		GetOut
	};

	struct PlayerInfo
	{
		std::string id;
		std::string streamName;
		float position[3] = { 0,0,0 };
		float rotation = 0;
	};

	void start_listen() override;
	bool listen_event(const std::string& i_eventName,const ddk::function<bool(const MessageResponse&)>& i_callback) override;
	bool send_event(const IOrchestratorMessage&,const ddk::function<void(msg_result)>& i_ackCallback) override;
	bool register_stream(const std::string&) override;
	void stop_listen() override;
	bool is_listening() const override;

	bool connect();
	void onConnectionStateChanged(IClientConnection::State i_oldState,IClientConnection::State i_newState);
	void onNewResponse(const MessageResponse& i_response);
	bool onNewPlayer(const MessageResponse& i_response);
	void send(const IOrchestratorMessage& i_msg);
	bool waitForAck();
	ddk::optional<MessageResponse> waitForResponse();
	void onAck(msg_result i_error) const;
	void processResponse(const MessageResponse& i_response);
	bool goToNextState();
	void update();
	void informAboutNewUser(const PlayerInfo& i_response);

	ddk::signal<void(client_connection_lent_ptr)> sig_onSocketChanged;
	std::string m_url;
	player_id m_id;
	std::string m_userId;
	std::string m_sessionId;
	std::vector<PlayerInfo> m_registeredPlayers;
	fov_data m_fov = { 0 };
	lod_data m_lod = { 0 };
	position_3d m_position = { 0 };
	rotation_2d m_rotation = 0;
	client_connection_unique_ptr m_connection;
	ddk::thread_event_driven_executor m_executor;
	mutable pthread_mutex_t m_mutex;
	mutable pthread_cond_t m_condVar;
	mutable ddk::optional<msg_result> m_receivedAck = none;
	ddk::optional<MessageResponse> m_response;
	State m_state = Idle;
};

}