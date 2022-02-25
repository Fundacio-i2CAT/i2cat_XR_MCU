#pragma once

#include "IFrameWriter.h"
#include "IPlayerReceiver.h"
#include "ProfilerDefs.h"

namespace mcu
{

class SocketIoFrameWriter : public IFrameWriter
{
public:
	SocketIoFrameWriter(const frame_dest_id& i_id);
	~SocketIoFrameWriter();

private:
	send_result send(const EncodedFrame&) override;

	void onSocketChanged(client_connection_lent_ref i_connection);
	void onSessionStablished();

	const frame_dest_id m_id;
	player_receiver_lent_ptr m_playerReceiver = nullptr;
	const std::string m_exchangeName;
	std::string m_publishUrl;
	size_t m_numBytesSent;
	pfr::measure_id m_SentTotalBytesRateMeasId;
};

}