#include "CwipcEncodedFrame.h"

namespace mcu
{

CwipcEncodedFrame::CwipcEncodedFrame(cwipc_encoder* i_encoder)
: m_buffer(nullptr)
, m_size(0)
{
	m_size = i_encoder->get_encoded_size();

	if (m_buffer = malloc(m_size))
	{
		if (i_encoder->copy_data(m_buffer,m_size) == false)
		{
			m_size = 0;

			free(m_buffer);

			throw FusionEncoderException{};
		}
	}
	else
	{
		m_size = 0;

		throw FusionEncoderException{};
	}
}
CwipcEncodedFrame::CwipcEncodedFrame(CwipcEncodedFrame&& other)
: m_buffer(nullptr)
, m_size(0)
{
	std::swap(m_buffer,other.m_buffer);
	std::swap(m_size,other.m_size);
}
CwipcEncodedFrame::~CwipcEncodedFrame()
{
	if (m_buffer)
	{
		free(m_buffer);
	}
}
EncodecType CwipcEncodedFrame::get_type() const
{
	return EncodecType::Cwipc;
}
void* CwipcEncodedFrame::get_buffer()
{
	return m_buffer;
}
size_t CwipcEncodedFrame::get_size() const
{
	return m_size;
}

}