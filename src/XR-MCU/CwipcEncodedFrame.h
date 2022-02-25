#pragma once

#include "EncodedFrame.h"
#include "cwipc_codec/api.h"

namespace mcu
{

class CwipcEncodedFrame : public detail::IEncodedFrameImpl
{
public:
	CwipcEncodedFrame(cwipc_encoder* i_encoder);
	CwipcEncodedFrame(const CwipcEncodedFrame&) = delete;
	CwipcEncodedFrame(CwipcEncodedFrame&& other);
	~CwipcEncodedFrame();
	
	CwipcEncodedFrame& operator=(const CwipcEncodedFrame&) = delete;
	CwipcEncodedFrame& operator=(CwipcEncodedFrame&&) = delete;

private:
	EncodecType get_type() const override;
	void* get_buffer() override;
	size_t get_size() const override;

	void* m_buffer;
	size_t m_size;
};

}