#include "MCUMeasurementMgr.h"
#include "IMCUProfiler.h"
#include "FusionModuleProfiler.h"
#include "AcquisitionModuleProfiler.h"
#include <cstdio>

namespace mcu
{

void MCUMeasurementMgr::Init()
{

	pfr::iprofiler_lent_ref mcuProfiler = pfr::IProfiler::get_instance();

	m_measuremntConn.push_back(mcuProfiler->listen_to<ReceivedFrameRateMeasurable>(k_acquisition_profile_module,[](const frame_receiver_fps& i_measure)
	{
		char msg[128] = { 0 };
		sprintf(msg,"Current received FPS from %s is %f",i_measure.get_id().c_str(),i_measure.get_fps());
		DDK_LOG_INFO(msg);
	}));

	//m_measuremntConn.push_back(mcuProfiler->listen_to<ReceivedTotalBytesRateMeasurable>(k_acquisition_profile_module,[](float i_measure)
	//{
	//	DDK_LOG_INFO("current Received Total Bytes rate: " << i_measure);
	//}));

	m_measuremntConn.push_back(mcuProfiler->listen_to<DecodedFrameRateMeasurable>(k_acquisition_profile_module,[](const frame_decoder_fps& i_measure)
	{
		char msg[128] = { 0 };
		sprintf(msg,"Current decoded FPS from %s is %f",i_measure.get_id().getValue().c_str(),i_measure.get_fps());
		DDK_LOG_INFO(msg);
	}));

	//m_measuremntConn.push_back(mcuProfiler->listen_to<PointcloudSizeMeasurable>(k_acquisition_profile_module,[](const pointcloud_size& i_measure)
	//{
	//	char msg[128] = { 0 };
	//	sprintf(msg,"Decoded pointcloud from %s has size %zd",i_measure.get_id().getValue().c_str(),i_measure.get_size());
	//	DDK_LOG_INFO(msg);
	//}));

	m_measuremntConn.push_back(mcuProfiler->listen_to<DecodedFrameLatencyMeasurable>(k_acquisition_profile_module,[](const frame_latency& i_measure)
	{
		char msg[128] = { 0 };
		sprintf(msg,"Decoded pointcloud from %s has latency %lld",i_measure.get_id().getValue().c_str(),i_measure.get_latency().count());
		DDK_LOG_INFO(msg);
	}));

	m_measuremntConn.push_back(mcuProfiler->listen_to<DecodingTimeMeasurable>(k_acquisition_profile_module,[](const decoding_time& i_measure)
	{
		char msg[128] = { 0 };
		sprintf(msg, "Decoding time from %s is %lld", i_measure.get_id().getValue().c_str(), i_measure.get_duration().count());
		DDK_LOG_INFO(msg);
	}));

	m_measuremntConn.push_back(mcuProfiler->listen_to<TransformTimeMeasurable>(k_acquisition_profile_module,[](const transform_duration_time& i_measure)
	{
		char msg[128] = { 0 };
		sprintf(msg,"Transform time from %s is %lld",i_measure.get_id().getValue().c_str(),i_measure.get_duration().count());
		DDK_LOG_INFO(msg);
	}));

	m_measuremntConn.push_back(mcuProfiler->listen_to<TransformedFrameRateMeasurable>(k_acquisition_profile_module,[](const frame_decoder_fps& i_measure)
	{
		char msg[128] = { 0 };
		sprintf(msg,"Current transform FPS from %s is %f",i_measure.get_id().getValue().c_str(),i_measure.get_fps());
		DDK_LOG_INFO(msg);
	}));

	//m_measuremntConn.push_back(mcuProfiler->listen_to<DroppedDecodedFrameRatio>(k_fusion_profile_module,[](float i_dropRatio)
	//{
	//	DDK_LOG_INFO("Ratio of dropped decoded frames: " << i_dropRatio);
	//}));

	m_measuremntConn.push_back(mcuProfiler->listen_to<FusionTimeMeasurable>(k_fusion_profile_module,[](const fusion_duration_time& i_measure)
	{
		DDK_LOG_INFO("Fusion time(milliseconds): " << i_measure.count());
	}));

	m_measuremntConn.push_back(mcuProfiler->listen_to<EncodingTimeMeasurable>(k_fusion_profile_module,[](const encoding_time& i_measure)
	{
		char msg[128] = { 0 };
		sprintf(msg,"Encoding time from %s is(milliseconds) %lld",i_measure.get_id().getValue().c_str(),i_measure.get_duration().count());
		DDK_LOG_INFO(msg);
	}));

	m_measuremntConn.push_back(mcuProfiler->listen_to<SendingTimeMeasurable>(k_fusion_profile_module,[](const sending_duration_time& i_measure)
	{
		DDK_LOG_INFO("Sending time(milliseconds): " << i_measure.count());
	}));

	m_measuremntConn.push_back(mcuProfiler->listen_to<FusionedFrameRateMeasurable>(k_fusion_profile_module,[](frame_fusioned_fps i_measure)
	{
		char msg[128] = { 0 };
		sprintf(msg,"Current fusion FPS from %s is %f",i_measure.get_id().getValue().c_str(),i_measure.get_fps());
		DDK_LOG_INFO(msg);
	}));

	m_measuremntConn.push_back(mcuProfiler->listen_to<FusionedFrameLatencyMeasurable>(k_fusion_profile_module,[](const frame_latency& i_measure)
	{
		char msg[128] = { 0 };
		sprintf(msg,"Fusioned pointcloud from %s has latency %lld",i_measure.get_id().getValue().c_str(),i_measure.get_latency().count());
		DDK_LOG_INFO(msg);
	}));

	m_measuremntConn.push_back(mcuProfiler->listen_to<SentFrameRateMeasurable>(k_fusion_profile_module,[](frame_sent_fps i_measure)
	{
		char msg[128] = { 0 };
		sprintf(msg,"Current sent FPS from %s is %f",i_measure.get_id().getValue().c_str(),i_measure.get_fps());
		DDK_LOG_INFO(msg);
	}));

	//m_measuremntConn.push_back(mcuProfiler->listen_to<SentTotalBytesRateMeasurable>(k_fusion_profile_module,[](float i_measure)
	//{
	//	DDK_LOG_INFO("current Sent Total Bytes rate: " << i_measure);
	//}));
}
void MCUMeasurementMgr::Deinit()
{
	m_measuremntConn.clear();
}

}