#include "B2DFrameWriter.h"
#include "FusionModuleProfiler.h"
#include "IMCUProfiler.h"
#include <chrono>

namespace mcu
{

B2DFrameWriter::B2DFrameWriter(const std::string& i_exchangeName, const std::string& i_publishUrl)
{
	const int numStreams = 1;
	StreamDesc* b2dDescriptors = new StreamDesc[numStreams];
	b2dDescriptors[0].MP4_4CC = VRT_4CC('c', 'w', 'i', '1');
	b2dDescriptors[0].tileNumber = 0;
	b2dDescriptors[0].quality = 0;

	m_b2dHandle = vrt_create_ext(i_exchangeName.c_str(), numStreams, b2dDescriptors, i_publishUrl.c_str(), 2000, 30000);
	//m_b2dHandle = vrt_create(i_exchangeName.c_str(), VRT_4CC('c', 'w', 'i', '1'), i_publishUrl.c_str(), 2000, 30000);

	if (m_b2dHandle == nullptr)
	{
		throw FusionSenderException{};
	}
}
B2DFrameWriter::~B2DFrameWriter()
{
	if (m_b2dHandle)
	{
		vrt_destroy(m_b2dHandle);
	}
}
B2DFrameWriter::send_result B2DFrameWriter::send(const EncodedFrame& i_frame)
{
	if (m_b2dHandle)
	{
		if (void* nestedBuffer = i_frame.get_buffer())
		{
			const pfr::profiler_time_point beforeOp = pfr::profiler_clock::now();

			const size_t bufferSize = i_frame.get_size();

			if (vrt_push_buffer(m_b2dHandle,reinterpret_cast<uint8_t*>(nestedBuffer),bufferSize))
			{
				PUBLISH_MEASURE(k_fusion_profile_module,SendingTimeMeasurable,std::make_pair(frame_dest_id("universal"),std::chrono::seconds(1)),std::chrono::duration_cast<std::chrono::milliseconds>(pfr::profiler_clock::now() - beforeOp))

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

}