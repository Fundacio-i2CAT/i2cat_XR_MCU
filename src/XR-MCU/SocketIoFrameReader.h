#pragma once

#include "IFrameReader.h"
#include <pthread.h>
#include "sio_socket.h"
#include "IClientConnection.h"
#include "ProfilerDefs.h"
#include "ddk_atomics.h"

namespace mcu
{

class SocketIoFrameReader : public IFrameReader
{
public:
	SocketIoFrameReader(const std::string& i_streamName,size_t& i_numReceivedBytes);
	~SocketIoFrameReader();

private:
	connect_result connect() override;
	read_result read() override;
	destroy_result destroy() override;
	bool is_connected() const override;

	bool onNewFrame(const MessageResponse& i_event);
	void onSocketChanged(client_connection_lent_ref i_connection);
	frame_receiver_fps compute_frame_rate(const pfr::profiler_time_point& i_time) const;

	const std::string m_url;
	const std::string m_streamName;
	frame_data m_newFrame;
	mutable pthread_mutex_t m_mutex;
	bool m_connected = false;
	size_t& m_numReceivedBytes;
	pfr::measure_id m_ReceivedFrameRateMeasId;
	mutable ddk::atomic_size_t m_numReceivedFrames = 0;
	mutable pfr::profiler_time_point m_lastFrameRateCheckTime;
};

}