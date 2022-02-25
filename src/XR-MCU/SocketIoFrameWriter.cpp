#include "SocketIoFrameWriter.h"
#include "IPlayerReceiver.h"
#include "OrchestratorMessages.h"
#include "FusionModuleProfiler.h"
#include "IMCUProfiler.h"

namespace mcu
{

SocketIoFrameWriter::SocketIoFrameWriter(const frame_dest_id& i_id)
: m_id(i_id)
, m_publishUrl(i_id.getValue() + "mcu#0")
{
	START_MEASURE(k_fusion_profile_module,pfr::make_measurable<SentTotalBytesRateMeasurable>(m_numBytesSent,std::chrono::seconds(1)),m_SentTotalBytesRateMeasId)
		
	if(m_playerReceiver = IPlayerReceiver::get_instance())
	{
		if(m_playerReceiver->is_listening())
		{
			m_playerReceiver->register_stream(m_publishUrl);
		}
		else
		{
			m_playerReceiver->sig_onSessionStablished.connect(ddk::make_function(this,&SocketIoFrameWriter::onSessionStablished));
		}
	}

}
SocketIoFrameWriter::~SocketIoFrameWriter()
{
	STOP_MEASURE(k_fusion_profile_module,m_SentTotalBytesRateMeasId);
}
typename SocketIoFrameWriter::send_result SocketIoFrameWriter::send(const EncodedFrame& i_frame)
{
	if(m_playerReceiver)
	{
		if(void* nestedBuffer = i_frame.get_buffer())
		{
			const pfr::profiler_time_point beforeOp = pfr::profiler_clock::now();

			const size_t bufferSize = i_frame.get_size();

			if(m_playerReceiver->send_event(make_message<OrchestratorFusionedFrame>(m_publishUrl,nestedBuffer,bufferSize)))
			{
				PUBLISH_MEASURE(k_fusion_profile_module,SendingTimeMeasurable,std::make_pair(m_id,std::chrono::seconds(1)),std::chrono::duration_cast<std::chrono::milliseconds>(pfr::profiler_clock::now() - beforeOp))

				m_numBytesSent += bufferSize;

				return bufferSize;
			}
			else
			{
				return ddk::make_error<send_result>(WriterSendFrameErrorCode::SendError);
			}
		}
		else
		{
			return ddk::make_error<send_result>(WriterSendFrameErrorCode::EmptyBuffer);
		}
	}
	else
	{
		return ddk::make_error<send_result>(WriterSendFrameErrorCode::NotAvailable);
	}
}
void SocketIoFrameWriter::onSessionStablished()
{
	if(m_playerReceiver)
	{
		m_playerReceiver->register_stream(m_publishUrl);
	}
}

}