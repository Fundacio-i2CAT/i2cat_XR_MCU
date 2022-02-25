#pragma once

#include "IPlayer.h"
#include "ddk_thread_executor.h"
#include "IFrameReader.h"
#include "MCUConfig.h"
#include "IClientConnection.h"

namespace mcu
{

class Player : public IPlayer, public ddk::static_visitor<bool>
{
public:
	Player(const player_id& i_id, const FrameReaderConfig& i_config,const FieldOfViewConfig& i_fovConfig);
	Player(const Player&) = delete;
	Player(Player&&) = default;

	void init(frame_reader_unique_ref i_frameReader, const PlayerData& i_msg);
	bool operator()(const std::array<float,3>& i_pos);
	bool operator()(float i_rot);
	bool operator()(Frustrum i_frustrum);
	bool operator()(const std::string& i_url);

private:
	void start() override;
	void update(const PlayerUpdateMessage&) override;
	void stop() override;
	const transformation& get_transformation() const override;
	Frustrum get_frustrum() const override;
	void notify() const override;

	void checkForNewFrames();
	bool onPlayerChanged(const MessageResponse& i_event);

	const std::string m_id;
	frame_reader_unique_ptr m_frameReader;
	ddk::thread_polling_executor m_updateExecutor;
	std::string m_url;
	transformation m_transform;
	Frustrum m_frustrum;
	std::string m_publicUrl;
};

}