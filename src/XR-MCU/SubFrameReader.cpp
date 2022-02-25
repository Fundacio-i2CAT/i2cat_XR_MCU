#include "SubFrameReader.h"
#include "DDKLogger.h"
#include "IMCUProfiler.h"
#include "AcquisitionModuleProfiler.h"

namespace mcu
{

SubFrameReader::SubFrameReader()
: m_subHandle(nullptr)
, m_id(0)
, m_isPlaying(false)
, m_streamCount(0)
, m_numberOfUnsuccessfulReceives(0)
, m_numReceivedFrames(0)
, m_ReceivedFrameRateMeasId(0)
{
	//START_MEASURE(k_acquisition_profile_module,pfr::make_measurable<ReceivedFrameRateMeasurable>(m_numReceivedFrames,std::chrono::seconds(5)),m_ReceivedFrameRateMeasId)
}
SubFrameReader::~SubFrameReader()
{
	//STOP_MEASURE(k_acquisition_profile_module,m_ReceivedFrameRateMeasId);
}
SubFrameReader::init_result SubFrameReader::init(const sub_reader_init_context& i_initContext)
{
	// SUB Init
	//m_url = i_initContext.get_url() + "pointcloud.mpd";
	m_url = i_initContext.get_url();
	m_id = i_initContext.get_id();

	if (m_subHandle == nullptr)
	{
		const std::string pipe = "MyPipeline_" + ddk::formatter<std::string>::format(m_id);

		m_subHandle = sub_create(pipe.c_str(),nullptr, SUB_API_VERSION);

		if (m_subHandle != nullptr)
		{
			return ddk::success;
		}
		else
		{
			DDK_LOG_ERROR(pipe << ": sub_create(" << m_url << ") failed");

			return ddk::make_error<init_result>(InitHandleCreateError);
		}
	}
	else
	{
		return ddk::make_error<init_result>(InitHandleAlreadyCreatedError);
	}
}
SubFrameReader::connect_result SubFrameReader::connect()
{
	if (m_isPlaying == false)
	{
		const std::string pipe = "SUB Reader " + ddk::formatter<std::string>::format(m_id);

		if (m_subHandle == nullptr)
		{
			return ddk::make_error<connect_result>(ConnectInvalidHandleError);
		}
		else
		{
			m_isPlaying = sub_play(m_subHandle, m_url.c_str());

			if (m_isPlaying == false)
			{
				DDK_LOG_ERROR(pipe << " Retry: sub_play(" << m_url << ") failed, will try again later");

				return ddk::make_error<connect_result>(ConnectHandlePlayError);
			}

			m_streamCount = sub_get_stream_count(m_subHandle);

			DDK_LOG_INFO(pipe << " Retry: streamCount = " << m_streamCount);

			m_numberOfUnsuccessfulReceives = 0;

			return ddk::success;
		}
	}
	else
	{
		return ddk::make_error<connect_result>(ConnectAlreadyConnected);
	}
}
SubFrameReader::read_result SubFrameReader::read()
{
	FrameInfo info{};

	// SUB
	if(m_subHandle)
	{
		const size_t bytesNeeded = sub_grab_frame(m_subHandle, 0, nullptr, 0, &info); // Get buffer length.

		if(bytesNeeded > 0)
		{
			//JAUME: this is commented since last changes for empowering performance does handle well this superseeded code (now handled by socketio).
			//frame_arena frame;
			//frame.resize(bytesNeeded);
			//const size_t bytesRead = sub_grab_frame(m_subHandle,0,frame.data(),bytesNeeded,&info);

			//if (bytesRead == bytesNeeded)
			//{
			//	++m_numReceivedFrames;

			//	return ddk::make_result<read_result>(std::make_pair(std::chrono::milliseconds(info.timestamp),std::move(frame)));
			//}
			//else
			//{
			//	return ddk::make_error<read_result>(ReadBytesReadMismatchError);
			//}
		} 
		else
		{
			++m_numberOfUnsuccessfulReceives;

			if (m_numberOfUnsuccessfulReceives > 300)
			{
				reconnect();

				m_numberOfUnsuccessfulReceives = 0;
			}

			return ddk::make_error<read_result>(ReadNoFrameAvailableError);
		}
	}
	else
	{
		return ddk::make_error<read_result>(ReadInvalidHandle);
	}
}
SubFrameReader::destroy_result SubFrameReader::destroy()
{
	if (m_subHandle)
	{
		sub_destroy(m_subHandle);

		return ddk::success;
	}
	else
	{
		return ddk::make_error<destroy_result>(DestroyInvalidHandle);
	}
}
bool SubFrameReader::is_connected() const
{
	return m_isPlaying;
}
void SubFrameReader::reconnect()
{
	if (m_subHandle)
	{
		sub_destroy(m_subHandle);
		
		m_subHandle = nullptr;				
	}

	const std::string pipe = "MyPipeline_" + ddk::formatter<std::string>::format(m_id);
		
	m_subHandle = sub_create(pipe.c_str(),nullptr, SUB_API_VERSION);
		
	m_isPlaying = sub_play(m_subHandle, m_url.c_str());

	m_streamCount = sub_get_stream_count(m_subHandle);

	DDK_LOG_INFO(pipe << " Retry: streamCount = " << m_streamCount);
}

}