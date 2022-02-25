#include "MCUConfig.h"
#include "ddk_formatter.h"
#include <set>

namespace mcu
{

TcpSocketConfig::TcpSocketConfig(unsigned int i_port, unsigned int i_pollingTime)
: m_port(i_port)
, m_pollingTime(i_pollingTime)
{
}
unsigned int TcpSocketConfig::get_port() const
{
	return m_port;
}
unsigned int TcpSocketConfig::get_polling_time() const
{
	return m_pollingTime;
}

ConnectionConfig::ConnectionConfig(Type i_type,const std::string& i_ip, unsigned int i_port)
: m_type(i_type)
, m_ip(i_ip)
, m_port(i_port)
{
}
ConnectionConfig::Type ConnectionConfig::get_type() const
{
	return m_type;
}
const std::string& ConnectionConfig::get_ip() const
{
	return m_ip;
}
unsigned int ConnectionConfig::get_port() const
{
	return m_port;
}

OrchestratorConfig::OrchestratorConfig(const ConnectionConfig& i_connConfig)
: m_connConfig(i_connConfig)
{
}
const ConnectionConfig& OrchestratorConfig::getConnectionConfig() const
{
	return m_connConfig;
}

FrameReaderConfig::FrameReaderConfig(unsigned int i_pollingTime)
: m_pollingTime(i_pollingTime)
{
}
unsigned int FrameReaderConfig::get_polling_time() const
{
	return m_pollingTime;
}

DecoderBanckConfig::DecoderBanckConfig(size_t i_numDecoderThreads, size_t i_maxNumPendingFrames)
: m_numDecoderThreads(i_numDecoderThreads)
, m_maxNumPendingFrame(i_maxNumPendingFrames)
{
}
size_t DecoderBanckConfig::getNumDecoderThreads() const
{
	return m_numDecoderThreads;
}
size_t DecoderBanckConfig::getMaxNumPendingFrames() const
{
	return m_maxNumPendingFrame;
}

FrameWriterConfig::FrameWriterConfig(const std::string& i_url, const std::string& i_exchangeName)
: m_url(i_url)
, m_exchangeName(i_exchangeName)
{
}
std::string FrameWriterConfig::getUrl() const
{
	return m_url;
}
std::string FrameWriterConfig::getExchangeName() const
{
	return m_exchangeName;
}

LodConfig::LodConfig(unsigned int i_level, int i_numerator,unsigned int i_denominator)
: m_level(i_level)
, m_ratio(i_numerator,i_denominator)
{
}
bool LodConfig::operator==(const LodConfig& other) const
{
	return m_level == other.m_level;
}
float LodConfig::get_ratio() const
{
	return static_cast<float>(m_ratio.first) / m_ratio.second;
}
std::pair<int,unsigned int> LodConfig::get_fraction() const
{
	return m_ratio;
}
unsigned int LodConfig::get_level() const
{
	return m_level;
}

FieldOfViewConfig::FieldOfViewConfig(float i_primaryAreaDistance,unsigned int i_mainFov,unsigned int i_totalFov)
: m_primaryAreaDistance(i_primaryAreaDistance)
, m_mainFov(i_mainFov)
, m_totalFov(i_totalFov)
{
}
float FieldOfViewConfig::get_primary_area_distance() const
{
	return m_primaryAreaDistance;
}
unsigned int FieldOfViewConfig::get_main_fov() const
{
	return m_mainFov;
}
unsigned int FieldOfViewConfig::get_total_fov() const
{
	return m_totalFov;
}

McuConfig::McuConfig(const std::set<ReceiverType>& i_receiverTypes, size_t i_maxNumPendingDecodedFrames, size_t i_numPendingEncodedFramesToSent, size_t i_numEncoderThreads, unsigned int i_frameRate,float i_primaryAreaDistance,unsigned int i_mainFov,unsigned int i_totalFov)
: m_receiverTypes(i_receiverTypes)
, m_maxNumPendingDecodedFrames(i_maxNumPendingDecodedFrames)
, m_numPendingEncodedFramesToSent(i_numPendingEncodedFramesToSent)
, m_numEncoderThreads(i_numEncoderThreads)
, m_frameRate(i_frameRate)
, m_fieldOfView({ i_primaryAreaDistance,i_mainFov,i_totalFov })
{
}
McuConfig::const_receiver_iterator McuConfig::begin_receiver_types() const
{
	return m_receiverTypes.begin();
}
McuConfig::const_receiver_iterator McuConfig::end_receiver_types() const
{
	return m_receiverTypes.end();
}
size_t McuConfig::get_max_num_decoded_pending_frames() const
{
	return m_maxNumPendingDecodedFrames;
}
size_t McuConfig::get_num_encoded_frames_to_sent() const
{
	return m_numPendingEncodedFramesToSent;
}
size_t McuConfig::get_num_encoder_trheads() const
{
	return m_numEncoderThreads;
}
unsigned int McuConfig::get_frame_rate() const
{
	return m_frameRate;
}
const FieldOfViewConfig& McuConfig::get_field_of_view_config() const
{
	return m_fieldOfView;
}

}