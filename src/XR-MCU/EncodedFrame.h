#pragma once

#include "FrameEncoderDefs.h"
#include "ddk_shared_reference_wrapper.h"

namespace mcu
{
namespace detail
{

class IEncodedFrameImpl
{
public:
	virtual ~IEncodedFrameImpl() = default;
	virtual EncodecType get_type() const = 0;
	virtual void* get_buffer() = 0;
	virtual size_t get_size() const = 0;
};

}

class EncodedFrame
{
public:
	EncodedFrame(const EncodedFrame&) = default;
	EncodedFrame(EncodedFrame&& other) = default;
	~EncodedFrame();

	EncodedFrame& operator=(const EncodedFrame&) = delete;
	EncodedFrame& operator=(EncodedFrame&& other);

	template<typename Encoder, typename ... Args>
	static ddk::distributed_reference_wrapper<EncodedFrame> create_impl(frame_dest_id i_id, mcu_frame_time_point i_timeStamp, Args&& ... i_args)
	{
		static_assert(std::is_base_of<detail::IEncodedFrameImpl,Encoder>::value, "Encoder does not inherit from expected interface");

		return ddk::make_distributed_reference<EncodedFrame>(i_id, i_timeStamp, new Encoder(std::forward<Args>(i_args) ...));
	}
	mcu_frame_time_point get_time_stamp() const;
	frame_dest_id get_id() const;
	template<typename Encoder>
	Encoder* get_impl() const
	{
		static_assert(std::is_base_of<detail::IEncodedFrameImpl,Decoder>::value, "Trying to cast unrelated types");

		return dynamic_cast<Encoder*>(m_impl);
	}
	void* get_buffer() const;
	size_t get_size() const;

	EncodedFrame(frame_dest_id i_id, mcu_frame_time_point i_timeStamp, detail::IEncodedFrameImpl* i_impl);

private:
	frame_dest_id m_id;
	mcu_frame_time_point m_timeStamp;
	detail::IEncodedFrameImpl* m_impl;
};

typedef ddk::distributed_reference_wrapper<EncodedFrame> encoded_frame_dist_ref;
typedef ddk::distributed_reference_wrapper<const EncodedFrame> encoded_frame_const_dist_ref;
typedef ddk::distributed_pointer_wrapper<EncodedFrame> encoded_frame_dist_ptr;
typedef ddk::distributed_pointer_wrapper<const EncodedFrame> encoded_frame_const_dist_ptr;

}