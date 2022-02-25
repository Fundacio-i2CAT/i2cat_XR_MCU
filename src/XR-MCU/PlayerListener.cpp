#include "PlayerListener.h"
#include "TcpSocketPlayerReceiver.h"
#include "SocketIoPlayerReceiver.h"

namespace mcu
{

listener_bad_register::listener_bad_register(const std::string& i_reason)
: m_reason(i_reason)
{
}
const char* listener_bad_register::what() const
{
	return m_reason.c_str();
}

void PlayerListener::register_listener(ReceiverType i_recType, const MCUReceiverConfigProvider& i_configProvider)
{
	DDK_ASSERT(m_stop == false, "Starting listeners with player listener already stopped");

	player_receiver_unique_ptr newListener;

	switch (i_recType)
	{
		case ReceiverType::TcpSocket:
		{
			//newListener = ddk::make_unique_reference<TcpSocketPlayerReceiver>(i_configProvider);

			break;
		}
		case ReceiverType::SocketIo:
		{
			newListener = ddk::make_unique_reference<SocketIoPlayerReceiver>(i_configProvider);

			break;
		}
		default:
		{
			throw listener_bad_register("Receiver type not supported" + std::string(get_enum_name(i_recType)));
		}
	}

	newListener->sig_onPlayerCreated.connect(ddk::make_function(this,&PlayerListener::onPlayerCreated));
	newListener->sig_onPlayerUpdated.connect(ddk::make_function(this,&PlayerListener::onPlayerUpdated));
	newListener->sig_onPlayerDeleted.connect(ddk::make_function(this,&PlayerListener::onPlayerDeleted));

	m_listeners.emplace_back(ddk::promote_to_ref(std::move(newListener)));
}
void PlayerListener::start()
{
	DDK_ASSERT(m_stop == false, "Starting listeners with player listener already stopped");

	player_listeners::iterator itListener = m_listeners.begin();
	for(;itListener!=m_listeners.end();++itListener)
	{
		(*itListener)->start_listen();
	}
}
void PlayerListener::stop()
{
	DDK_ASSERT(m_stop == false, "Starting listeners with player listener already stopped");

	m_stop = true;

	player_listeners::iterator itListener = m_listeners.begin();
	for(;itListener!=m_listeners.end();++itListener)
	{
		(*itListener)->stop_listen();
	}
}
void PlayerListener::onPlayerCreated(player_id i_id, const PlayerCreateMessage& i_msg) const
{
	sig_onPlayerCreated.execute(i_id,i_msg);
}
void PlayerListener::onPlayerUpdated(player_id i_id, const PlayerUpdateMessage& i_msg) const
{
	sig_onPlayerUpdated.execute(i_id,i_msg);
}
void PlayerListener::onPlayerDeleted(player_id i_id) const
{
	sig_onPlayerDeleted.execute(i_id);
}

}