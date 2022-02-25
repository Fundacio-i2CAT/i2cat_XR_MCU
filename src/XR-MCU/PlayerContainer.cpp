#include "PlayerContainer.h"
#include "ddk_reference_wrapper.h"
#include "Player.h"
#include "SubFrameReader.h"
#include "SocketIoFrameReader.h"
#include "IMCUProfiler.h"
#include "AcquisitionModuleProfiler.h"

namespace mcu
{

PlayerContainer::PlayerContainer(const MCUFrameReaderConfigProvider& i_configProvider)
: m_frameReaderConfigProvider(i_configProvider)
{
	START_MEASURE(k_acquisition_profile_module,pfr::make_measurable<ReceivedTotalBytesRateMeasurable>(m_numReceivedBytes,std::chrono::seconds(1)),m_ReceivedTotalBytesRateMeasId)
}
PlayerContainer::~PlayerContainer()
{
	STOP_MEASURE(k_acquisition_profile_module,m_ReceivedTotalBytesRateMeasId);

	Shutdown();
}
void PlayerContainer::Shutdown()
{
	ddk::critical_section_context ctxt = enterWriter(ddk::Reentrancy::NON_REENTRANT);

	player_container::iterator itPlayer = m_players.begin();
	for (;itPlayer!=m_players.end();++itPlayer)
	{
		itPlayer->second->stop();
	}

	leaveWriter(std::move(ctxt));
}
PlayerContainer::create_player_result PlayerContainer::CreatePlayer(player_id i_id, const PlayerCreateMessage& i_msg,const FieldOfViewConfig& i_fovConfig)
{
	frame_reader_unique_ptr reader;
	const PlayerData playerData = i_msg.get_data();

	ddk::unique_reference_wrapper<Player> newPlayer = ddk::make_unique_reference<Player>(i_id,m_frameReaderConfigProvider.getConfig(i_msg.get_reader_type()),i_fovConfig);

	if (frame_reader_unique_ptr reader = create_frame_reader(i_id,i_msg.get_reader_type(), playerData))
	{
		newPlayer->init(ddk::promote_to_ref(std::move(reader)),playerData);

		iplayer_lent_ref res = ddk::lend(newPlayer);

		ddk::critical_section_context ctxt = enterWriter(ddk::Reentrancy::NON_REENTRANT);

		const std::pair<private_iterator,bool> insertRes = m_players.insert(std::make_pair(i_id,std::move(newPlayer)));

		leaveWriter(std::move(ctxt));

		if(insertRes.second)
		{
			DDK_LOG_INFO("Player " << i_id.getValue() <<" created succesfully");

			return res;
		}
		else
		{
			DDK_LOG_ERROR("Player " << i_id << "could not be created: id already present");

			return ddk::make_error<create_player_result>(CreatePlayerErrorCode::CreatePlayerAlreadyExist);
		}
	}
	else
	{
		DDK_LOG_ERROR("Player " << i_id << "could not be created: could not create frame reader");

		return ddk::make_error<create_player_result>(CreatePlayerErrorCode::CreatePlayerReaderNotImplmented);
	}
}
PlayerContainer::update_player_result PlayerContainer::UpdatePlayer(player_id i_id, const PlayerUpdateMessage& i_msg)
{
	ddk::critical_section_context ctxt = enterWriter(ddk::Reentrancy::NON_REENTRANT);

	player_container::iterator itPlayer = m_players.find(i_id);

	if (m_players.find(i_id) != m_players.end())
	{
		itPlayer->second->update(i_msg);

		leaveWriter(std::move(ctxt));

		return ddk::success;
	}
	else
	{
		leaveWriter(std::move(ctxt));

		return ddk::make_error<update_player_result>(UpdatePlayerErrorCode::UpdatePlayerNonExist);
	}
}
PlayerContainer::delete_player_result PlayerContainer::DeletePlayer(player_id i_id)
{
	ddk::critical_section_context ctxt = enterWriter(ddk::Reentrancy::NON_REENTRANT);

	player_container::iterator itPlayer = m_players.find(i_id);

	if (itPlayer != m_players.end())
	{
		m_players.erase(itPlayer);

		leaveWriter(std::move(ctxt));

		return ddk::success;
	}
	else
	{
		leaveWriter(std::move(ctxt));

		return ddk::make_error<delete_player_result>(DeletePlayerErrorCode::DeletePlayerNonExist);
	}
}
frame_reader_unique_ptr PlayerContainer::create_frame_reader(player_id i_id, FrameReaderType i_type, const PlayerData& i_playerData) const
{
	frame_reader_unique_ptr res;

	switch (i_type)
	{
		case FrameReaderType::Sub:
		{
			ddk::unique_pointer_wrapper<SubFrameReader> subReader = ddk::make_unique_reference<SubFrameReader>();

			const sub_reader_init_context readerInitCtxt(i_playerData.m_url,ReaderId(i_id.getValue()));
			SubFrameReader::init_result initRes = subReader->init(readerInitCtxt);

			if (initRes)
			{
				return std::move(subReader);
			}

			break;
		}
		case FrameReaderType::Socketio:
		{		
			return ddk::make_unique_reference<SocketIoFrameReader>(i_playerData.m_url,m_numReceivedBytes);
		}
		default:
		{
			DDK_FAIL_OR_LOG("Unsupported Reade mode type");
		}
	}

	return nullptr;
}
PlayerContainer::const_iterator PlayerContainer::begin() const
{
	return const_iterator(m_players.begin(),[](const std::pair<const player_id,iplayer_unique_ref>& i_pair){ return ddk::lend(i_pair.second); });
}
PlayerContainer::const_iterator PlayerContainer::end() const
{
	return const_iterator(m_players.end(),[](const std::pair<const player_id,iplayer_unique_ref>& i_pair){ return ddk::lend(i_pair.second); });
}
PlayerContainer::const_iterator PlayerContainer::find(player_id i_id) const
{
	return const_iterator(m_players.find(i_id),[](const std::pair<const player_id,iplayer_unique_ref>& i_pair){ return ddk::lend(i_pair.second); });
}
PlayerContainer::iterator PlayerContainer::begin()
{
	return iterator(m_players.begin(),[](const std::pair<const player_id,iplayer_unique_ref>& i_pair){ return ddk::lend(i_pair.second); });
}
PlayerContainer::iterator PlayerContainer::end()
{
	return iterator(m_players.end(),[](const std::pair<const player_id,iplayer_unique_ref>& i_pair){ return ddk::lend(i_pair.second); });
}
PlayerContainer::iterator PlayerContainer::find(player_id i_id)
{
	return iterator(m_players.find(i_id),[](const std::pair<const player_id,iplayer_unique_ref>& i_pair){ return ddk::lend(i_pair.second); });
}
PlayerContainer::critical_context PlayerContainer::enterReader(ddk::Reentrancy i_reentrancy) const
{
	m_exclArea.enterReader(i_reentrancy);

	return ddk::context_acquired;
}
PlayerContainer::critical_context PlayerContainer::tryToEnterReader(ddk::Reentrancy i_reentrancy) const
{
	return (m_exclArea.tryToEnterReader(i_reentrancy)) ? ddk::context_acquired : ddk::context_unacquired;
}
void PlayerContainer::leaveReader(critical_context i_context) const
{
	m_exclArea.leaverReader();
}
PlayerContainer::critical_context PlayerContainer::enterWriter(ddk::Reentrancy i_reentrancy)
{
	m_exclArea.enterWriter(i_reentrancy);

	return ddk::context_acquired;
}
PlayerContainer::critical_context PlayerContainer::tryToEnterWriter(ddk::Reentrancy i_reentrancy)
{
	return (m_exclArea.tryToEnterWriter(i_reentrancy)) ? ddk::context_acquired : ddk::context_unacquired;
}
void PlayerContainer::leaveWriter(critical_context)
{
	m_exclArea.leaveWriter();
}

}