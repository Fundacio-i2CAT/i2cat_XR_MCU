#pragma once

#include "IMCUManager.h"
#include "PlayerListener.h"
#include "MCUConfigProvider.h"
#include "PlayerContainer.h"
#include "MCUFusionManager.h"
#include "FrameDecoderBank.h"
#include "MCUMeasurementMgr.h"

namespace mcu
{

class MCUManager : public IMCUManager
{
public:
	MCUManager(const std::string& i_configFile);
	~MCUManager();

	void Init();
	int Exec();

private:
	void close() override;

	void onPlayerCreated(player_id i_id, const PlayerCreateMessage& i_msg);
	void onPlayerUpdated(player_id i_id, const PlayerUpdateMessage& i_msg);
	void onPlayerDeleted(player_id i_id);

	PlayerListener m_playerListener;
	player_container_unique_ptr m_playerContainer;
	fusion_manager_unique_ptr m_fusionMgr;
	pthread_mutex_t m_conVarMutex;
	pthread_cond_t m_condVar;
	bool m_stop;
	McuConfigProvider m_configProvider;
	ddk::unique_pointer_wrapper<FrameDecoderBank> m_decoderBank;
	MCUMeasurementMgr m_measuremtManager;
};


}