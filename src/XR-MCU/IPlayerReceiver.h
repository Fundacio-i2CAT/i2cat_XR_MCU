#pragma once

#include "ddkFramework/ddk_signal.h"
#include "ddkFramework/ddk_unique_reference_wrapper.h"
#include "PlayerDefs.h"
#include "OrchestratorMessages.h"
#include "IClientConnection.h"

namespace mcu
{

class IPlayerReceiver
{
public:
	virtual ~IPlayerReceiver() = default;

	virtual void start_listen() = 0;
	virtual bool listen_event(const std::string&,const ddk::function<bool(const MessageResponse&)>& i_callback) = 0;
	virtual bool send_event(const IOrchestratorMessage&,const ddk::function<void(msg_result)>& i_ackCallback = nullptr) = 0;
	virtual bool register_stream(const std::string&) = 0;
	virtual void stop_listen() = 0;

	virtual bool is_listening() const = 0;

	static ddk::lent_pointer_wrapper<IPlayerReceiver> get_instance();

	ddk::signal<void()> sig_onSessionStablished;
	ddk::signal<void(player_id, const PlayerCreateMessage&)> sig_onPlayerCreated;
	ddk::signal<void(player_id,const PlayerUpdateMessage&)> sig_onPlayerUpdated;
	ddk::signal<void(player_id)> sig_onPlayerDeleted;
	ddk::signal<void()> sig_onSessionAbandoned;

protected:
	static ddk::lent_pointer_wrapper<IPlayerReceiver> s_self;
};

typedef ddk::unique_reference_wrapper<IPlayerReceiver> player_receiver_unique_ref;
typedef ddk::unique_reference_wrapper<const IPlayerReceiver> player_receiver_const_unique_ref;
typedef ddk::unique_pointer_wrapper<IPlayerReceiver> player_receiver_unique_ptr;
typedef ddk::unique_pointer_wrapper<const IPlayerReceiver> player_receiver_const_unique_ptr;

typedef ddk::lent_reference_wrapper<IPlayerReceiver> player_receiver_lent_ref;
typedef ddk::lent_reference_wrapper<const IPlayerReceiver> player_receiver_const_lent_ref;
typedef ddk::lent_pointer_wrapper<IPlayerReceiver> player_receiver_lent_ptr;
typedef ddk::lent_pointer_wrapper<const IPlayerReceiver> player_receiver_const_lent_ptr;

}