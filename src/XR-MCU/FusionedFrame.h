#pragma once

#include "ddk_reference_wrapper_deleter.h"
#include "FrameDefs.h"
#include "DecodedFrame.h"
#include "IFusionProvider.h"
#include <map>

namespace mcu
{

class FusionedFrameVisitor;

namespace detail
{


class IFusionedFrameImpl
{
public:
	virtual ~IFusionedFrameImpl() = default;

	virtual bool merge(const IDecodedFrameImpl* i_frame, frame_source_id i_id, const mcu_frame_time_point& i_timeStamp) = 0;
	virtual void visit(FusionedFrameVisitor& i_visitor) = 0;
};

}

class DecodedFrame;

class FusionedFrame
{
public:
	typedef std::map<frame_source_id,mcu_frame_time_point>::const_iterator const_iterator;

	FusionedFrame() = default;
	FusionedFrame(frame_dest_id i_id,detail::IFusionedFrameImpl* i_impl,const ddk::resource_deleter_interface& i_deleter);
	FusionedFrame(frame_dest_id i_id,detail::IFusionedFrameImpl* i_impl,const ddk::resource_deleter_interface& i_deleter, const mcu_frame_time_point& i_timeStamp);
	FusionedFrame(const FusionedFrame&) = delete;
	FusionedFrame(FusionedFrame&& other);
	~FusionedFrame();

	FusionedFrame& operator=(const FusionedFrame&) = delete;
	FusionedFrame& operator=(FusionedFrame&& other);
	frame_dest_id get_id() const;
	mcu_frame_time_point get_time_stamp() const;
	bool merge(frame_source_id i_id, const DecodedFrame& i_frame);
	const frame_souce_time_stamp_list& get_src_time_stamps() const;
	explicit operator bool() const;
	void visit(FusionedFrameVisitor& i_visitor)
	{
		m_impl->visit(i_visitor);
	}
	template<typename Decoder>
	Decoder* get_impl() const
	{
		static_assert(std::is_base_of<detail::IFusionedFrameImpl,Decoder>::value, "Trying to cast unrelated types");

		return dynamic_cast<Decoder*>(m_impl);
	}

private:
	frame_dest_id m_id;
	detail::IFusionedFrameImpl* m_impl = nullptr;
	const ddk::resource_deleter_interface* m_deleter = nullptr;
	mcu_frame_time_point m_timeStamp;
	frame_souce_time_stamp_list m_srcTimeStamps;
};

}