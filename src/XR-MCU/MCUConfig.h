#pragma once

#include <string>
#include "FrameReaderDefs.h"
#include "ddk_scoped_enum.h"

namespace mcu
{

//receiver config

SCOPED_ENUM_DECL(ReceiverType,
	TcpSocket = 0,
	SocketIo
	)
SCOPED_ENUM_DECL_NAMES(ReceiverType,"TcpSocket", "Socket")

struct TcpSocketConfig
{
public:
	TcpSocketConfig(unsigned int i_port, unsigned int i_pollingTime);

	unsigned int get_port() const;
	unsigned int get_polling_time() const;

private:
	const unsigned int m_port;
	const unsigned int m_pollingTime;
};

struct ConnectionConfig
{
public:
	enum Type
	{
		Tcp,
		SocketIo
	};

	ConnectionConfig(Type i_type, const std::string& i_ip, unsigned int i_port);

	Type get_type() const;
	const std::string& get_ip() const;
	unsigned int get_port() const;

private:
	const Type m_type;
	const std::string m_ip;
	const unsigned int m_port;
};

struct OrchestratorConfig
{
public:
	OrchestratorConfig(const ConnectionConfig& i_connConfig);

	const ConnectionConfig& getConnectionConfig() const;

private:
	const ConnectionConfig m_connConfig;
};

//frame reader config

struct FrameReaderConfig
{
public:
	FrameReaderConfig(unsigned int i_pollingTime);

	unsigned int get_polling_time() const;

private:
	const unsigned int m_pollingTime;
};

//decoder config

struct DecoderBanckConfig
{
public:
	DecoderBanckConfig(size_t i_numDecoderThreads, size_t i_maxNumPendingFrames);

	size_t getNumDecoderThreads() const;
	size_t getMaxNumPendingFrames() const;

private:
	const size_t m_numDecoderThreads;
	const size_t m_maxNumPendingFrame;
};

struct FrameWriterConfig
{
public:
	FrameWriterConfig(const std::string& i_url, const std::string& i_exchangeName);

	std::string getUrl() const;
	std::string getExchangeName() const;

private:
	const std::string m_url;
	const std::string m_exchangeName;
};

struct LodConfig
{
	friend inline bool operator<(const LodConfig& i_lhs,float i_level)
	{
		return i_lhs.m_level < i_level;
	}
	friend inline bool operator<(float i_level,const LodConfig& i_rhs)
	{
		return i_level < i_rhs.m_level;
	}
public:
	LodConfig() = default;
	LodConfig(unsigned int i_level, int i_numerator, unsigned int i_denominator);

	bool operator==(const LodConfig& other) const;
	float get_ratio() const;
	std::pair<int,unsigned int> get_fraction() const;
	unsigned int get_level() const;

private:
	unsigned int m_level = 0;
	std::pair<int,unsigned int> m_ratio = {1,1};
};

struct FieldOfViewConfig
{
public:
	typedef std::vector<LodConfig>::const_iterator const_lod_iterator;
	typedef std::vector<LodConfig>::const_reverse_iterator const_lod_reverse_iterator;

	FieldOfViewConfig(float i_primaryAreaDistance, unsigned int i_mainFov, unsigned int i_totalFov);
	float get_primary_area_distance() const;
	unsigned int get_main_fov() const;
	unsigned int get_total_fov() const;

private:
	const float m_primaryAreaDistance;
	const unsigned int m_mainFov;
	const unsigned int m_totalFov;
};

//main config

struct McuConfig
{
public:
	typedef std::set<ReceiverType>::const_iterator const_receiver_iterator;

	McuConfig(const std::set<ReceiverType>& i_receiverTypes, size_t i_maxNumPendingDecodedFrames, size_t i_numPendingEncodedFramesToSent, size_t i_numEncoderThreads, unsigned int i_frameRateunsigned, float i_primaryAreaDistance,unsigned int i_mainFov,unsigned int i_totalFov);

	const_receiver_iterator begin_receiver_types() const;
	const_receiver_iterator end_receiver_types() const;
	size_t get_max_num_decoded_pending_frames() const;
	size_t get_num_encoded_frames_to_sent() const;
	size_t get_num_encoder_trheads() const;
	unsigned int get_frame_rate() const;
	const FieldOfViewConfig& get_field_of_view_config() const;

private:
	const std::set<ReceiverType> m_receiverTypes;
	const size_t m_maxNumPendingDecodedFrames;
	const size_t m_numPendingEncodedFramesToSent;
	const size_t m_numEncoderThreads;
	const unsigned int m_frameRate;
	const FieldOfViewConfig m_fieldOfView;
};

}