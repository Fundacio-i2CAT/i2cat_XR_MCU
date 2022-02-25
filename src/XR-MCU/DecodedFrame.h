#pragma once

#include "FrameDecoderDefs.h"
#include "FrameEncoderDefs.h"
#include "PlayerUtils.h"
#include <chrono>

namespace mcu
{

class DecodedFrameVisitor;

namespace detail
{

class IDecodedFrameImpl
{
public:
	virtual ~IDecodedFrameImpl() = default;
	virtual DecodecType get_type() const = 0;
	virtual void visit(DecodedFrameVisitor&) = 0;
	virtual void sample(float i_voxelSize) = 0;
	virtual void resize(size_t i_size) = 0;
	virtual bounding_box get_bounding_box() const = 0;
	virtual size_t size() const = 0;
	virtual bool empty() const = 0;
};

class DecodedFrameImpl : public IDecodedFrameImpl
{
public:
	DecodedFrameImpl() = default;

	void update_bounding_box(const bounding_box& i_boundingBox);

private:
	bounding_box get_bounding_box() const override;

	bounding_box m_boundingBox;
};

}

class DecodedFrame
{
public:
	DecodedFrame(const DecodedFrame&) = delete;
	DecodedFrame(DecodedFrame&& other);
	~DecodedFrame();

	DecodedFrame& operator=(const DecodedFrame&) = delete;
	DecodedFrame& operator=(DecodedFrame&& other);

	mcu_frame_time_point get_time_stamp() const;
	template<typename Decoder, typename ... Args>
	static DecodedFrame create_impl(const mcu_frame_time_point& i_timeStamp, Args&& ... i_args)
	{
		static_assert(std::is_base_of<detail::IDecodedFrameImpl,Decoder>::value, "Encoder does not inherit from expected interface");

		return { i_timeStamp, new Decoder(std::forward<Args>(i_args) ...) };
	}
	const detail::IDecodedFrameImpl* get_impl() const;
	bounding_box get_bounding_box() const;
	size_t size() const;
	bool empty() const;
	void visit(DecodedFrameVisitor&) const;

private:
	DecodedFrame(const mcu_frame_time_point& i_timestamp, detail::IDecodedFrameImpl*);

	mcu_frame_time_point m_timeStamp;
	detail::IDecodedFrameImpl* m_impl;
};

}