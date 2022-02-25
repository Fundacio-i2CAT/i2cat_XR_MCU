#pragma once

#include <cstdint>
#include <string>

struct CPacket
{
	//send
	friend CPacket& operator<<(CPacket& i_packet, bool i_data);
	friend CPacket& operator<<(CPacket& i_packet, int8_t i_data);
	friend CPacket& operator<<(CPacket& i_packet, uint8_t i_data);
	friend CPacket& operator<<(CPacket& i_packet, int16_t i_data);
	friend CPacket& operator<<(CPacket& i_packet, uint16_t i_data);
	friend CPacket& operator<<(CPacket& i_packet, int32_t i_data);
	friend CPacket& operator<<(CPacket& i_packet, uint32_t i_data);
	friend CPacket& operator<<(CPacket& i_packet, int64_t i_data);
	friend CPacket& operator<<(CPacket& i_packet, uint64_t i_data);
	friend CPacket& operator<<(CPacket& i_packet, float i_data);
	friend CPacket& operator<<(CPacket& i_packet, double i_data);
	friend CPacket& operator<<(CPacket& i_packet, char* i_data);
	friend CPacket& operator<<(CPacket& i_packet, const std::string& i_data);

	//reception
	friend CPacket& operator>>(CPacket& i_packet, bool& i_data);
	friend CPacket& operator>>(CPacket& i_packet, int8_t& i_data);
	friend CPacket& operator>>(CPacket& i_packet, uint8_t& i_data);
	friend CPacket& operator>>(CPacket& i_packet, int16_t& i_data);
	friend CPacket& operator>>(CPacket& i_packet, uint16_t& i_data);
	friend CPacket& operator>>(CPacket& i_packet, int32_t& i_data);
	friend CPacket& operator>>(CPacket& i_packet, uint32_t& i_data);
	friend CPacket& operator>>(CPacket& i_packet, int64_t& i_data);
	friend CPacket& operator>>(CPacket& i_packet, uint64_t& i_data);
	friend CPacket& operator>>(CPacket& i_packet, float& i_data);
	friend CPacket& operator>>(CPacket& i_packet, double& i_data);
	friend CPacket& operator>>(CPacket& i_packet, char* i_data);
	friend CPacket& operator>>(CPacket& i_packet, std::string& i_data);

public:
	CPacket();
	CPacket(const CPacket& other);
	CPacket(CPacket&& other);
	CPacket(uint8_t* i_data, size_t i_size);
	~CPacket();

	CPacket& operator=(const CPacket& other);
	CPacket& operator=(CPacket&& other);
	const uint8_t* get_data() const;
	size_t get_size() const;

private:
	CPacket(const CPacket& other, size_t i_size);

	bool expand(size_t i_size);

	uint8_t* m_data;
	size_t m_capacity;
	size_t m_currPos;
	bool m_borrowedMemory;
};