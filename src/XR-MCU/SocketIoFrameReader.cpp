#include "SocketIoFrameReader.h"
#include "SocketIoPlayerReceiver.h"
#include "IMCUProfiler.h"
#include "AcquisitionModuleProfiler.h"

namespace mcu
{

SocketIoFrameReader::SocketIoFrameReader(const std::string& i_streamName,size_t& i_numReceivedBytes)
: m_streamName(i_streamName)
, m_ReceivedFrameRateMeasId(0)
, m_numReceivedBytes(i_numReceivedBytes)
{
	pthread_mutex_init(&m_mutex,nullptr);

	IPlayerReceiver::get_instance()->listen_event("DataReceived",ddk::make_function(this,&SocketIoFrameReader::onNewFrame));

	START_MEASURE(k_acquisition_profile_module,pfr::make_measurable<ReceivedFrameRateMeasurable>(ddk::make_function(this,&SocketIoFrameReader::compute_frame_rate),std::chrono::seconds(1)),m_ReceivedFrameRateMeasId)
}
SocketIoFrameReader::~SocketIoFrameReader()
{
	STOP_MEASURE(k_acquisition_profile_module,m_ReceivedFrameRateMeasId);

	pthread_mutex_destroy(&m_mutex);
}
SocketIoFrameReader::connect_result SocketIoFrameReader::connect()
{
	m_connected = true;

	return ddk::success;
}
SocketIoFrameReader::read_result SocketIoFrameReader::read()
{
	pthread_mutex_lock(&m_mutex);

	if(m_newFrame.second.empty())
	{
		pthread_mutex_unlock(&m_mutex);

		return ddk::make_error<read_result>(ReadNoFrameAvailableError);
	}
	else
	{
		frame_data res = std::move(m_newFrame);

		pthread_mutex_unlock(&m_mutex);

		return res;
	}
}
SocketIoFrameReader::destroy_result SocketIoFrameReader::destroy()
{
	m_connected = false;
	
	return ddk::success;
}
bool SocketIoFrameReader::is_connected() const
{
	return m_connected;
}
bool SocketIoFrameReader::onNewFrame(const MessageResponse& i_event)
{
	if(m_connected)
	{
		//by construction we know that first message is the userId
		MessageResponse::const_iterator itMsg = i_event.begin();

		//second message is the streamName 
		++itMsg;
		if(itMsg->has_value(m_streamName))
		{
			//and third message is the binary data
			++itMsg;

			pthread_mutex_lock(&m_mutex);

			m_newFrame = std::make_pair(mcu_frame_clock::now(),std::move(itMsg->begin()->second));

			m_numReceivedBytes += m_newFrame.second.size();

			pthread_mutex_unlock(&m_mutex);

			ddk::atomic_post_increment(m_numReceivedFrames);

			return true;
		}
	}

	return false;
}
frame_receiver_fps SocketIoFrameReader::compute_frame_rate(const pfr::profiler_time_point& i_time) const
{
	const float frameRate = (m_lastFrameRateCheckTime != pfr::profiler_time_point()) ? static_cast<float>(m_numReceivedFrames.get()) / std::chrono::duration_cast<std::chrono::milliseconds>(i_time - m_lastFrameRateCheckTime).count() : m_numReceivedFrames.get();

	m_numReceivedFrames.set(0);
	m_lastFrameRateCheckTime = i_time;

	return { m_streamName,frameRate * 1000.f };
}

}