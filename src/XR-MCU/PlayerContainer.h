#pragma once

#include "IPlayerContainer.h"
#include "IPlayer.h"
#include "ddk_exclusion_area.h"
#include "FrameDecoderBank.h"
#include "ddk_unique_pointer_wrapper.h"
#include "IFrameReader.h"
#include "FrameReaderDefs.h"
#include "MCUConfigProvider.h"

namespace mcu
{

class PlayerContainer : public IPlayerProvider
{
public:
	enum class CreatePlayerErrorCode
	{
		CreatePlayerAlreadyExist,
		CreatePlayerReaderNotImplmented,
		CreatePlayerReaderError,
		CreatePlayerCreateDecoderError
	};
	typedef ddk::result<iplayer_lent_ref,CreatePlayerErrorCode> create_player_result;
	enum class UpdatePlayerErrorCode
	{
		UpdatePlayerNonExist
	};
	typedef ddk::result<void,UpdatePlayerErrorCode> update_player_result;
	enum class DeletePlayerErrorCode
	{
		DeletePlayerNonExist
	};
	typedef ddk::result<void,DeletePlayerErrorCode> delete_player_result;

	PlayerContainer(const MCUFrameReaderConfigProvider& i_readerFrameConfigProvider);
	~PlayerContainer();

	void Shutdown();
	create_player_result CreatePlayer(player_id, const PlayerCreateMessage&, const FieldOfViewConfig&);
	update_player_result UpdatePlayer(player_id, const PlayerUpdateMessage&);
	delete_player_result DeletePlayer(player_id);

private:
	frame_reader_unique_ptr create_frame_reader(player_id i_id, FrameReaderType i_type, const PlayerData& i_playerData) const;

	const_iterator begin() const override;
	const_iterator end() const override;
	const_iterator find(player_id i_id) const override;
	iterator begin() override;
	iterator end() override;
	iterator find(player_id i_id) override;

	critical_context enterReader(ddk::Reentrancy) const override;
	critical_context tryToEnterReader(ddk::Reentrancy) const override;
	void leaveReader(critical_context) const override;
	critical_context enterWriter(ddk::Reentrancy) override;
	critical_context tryToEnterWriter(ddk::Reentrancy) override;
	void leaveWriter(critical_context) override;

	player_container m_players;
	const MCUFrameReaderConfigProvider m_frameReaderConfigProvider;
	mutable ddk::exclusion_area m_exclArea;
	mutable size_t m_numReceivedBytes = 0;
	pfr::measure_id m_ReceivedTotalBytesRateMeasId;
};

typedef ddk::unique_pointer_wrapper<PlayerContainer> player_container_unique_ptr;
typedef ddk::unique_pointer_wrapper<const PlayerContainer> player_container_const_unique_ptr;

}