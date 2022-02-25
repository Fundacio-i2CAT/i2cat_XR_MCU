#pragma once

#include "IPlayerReceiver.h"
#include <list>
#include "MCUConfigProvider.h"

namespace mcu
{

struct listener_bad_register : public std::exception
{
public:
	listener_bad_register(const std::string& i_reason);
	
	const char* what() const override;

private:
	const std::string m_reason;
};

class PlayerListener
{
	typedef std::list<player_receiver_unique_ref> player_listeners;

public:
	PlayerListener() = default;

	void start();
	void stop();

	void register_listener(ReceiverType i_recType, const MCUReceiverConfigProvider& i_configProvider);

	ddk::signal<void(player_id, const PlayerCreateMessage&)> sig_onPlayerCreated;
	ddk::signal<void(player_id,const PlayerUpdateMessage&)> sig_onPlayerUpdated;
	ddk::signal<void(player_id)> sig_onPlayerDeleted;

private:
	void onPlayerCreated(player_id i_id, const PlayerCreateMessage& i_msg) const;
	void onPlayerUpdated(player_id i_id, const PlayerUpdateMessage& i_msg) const;
	void onPlayerDeleted(player_id i_id) const;

	player_listeners m_listeners;
	bool m_stop = false;
};

}