#pragma once

#include "FusionedFrame.h"
#include "FusionedFrameVisitor.h"
#include "CwipcDecodedFrame.h"

namespace mcu
{
namespace detail
{

struct FrameIdComparator
{
	bool operator()(const frame_source_id& i_lhs, const frame_source_id& i_rhs) const;
};

}

class FusionedFrameVisitor;

class CwipcFusionedFrame : protected CwipcDecodedFrame, public detail::IFusionedFrameImpl
{
public:
	CwipcFusionedFrame(cwipc* i_data);

	cwipc* getData();
	const cwipc* getData() const;

private:
	struct frame_info
	{
		size_t m_initPos;
		size_t m_size;
		mcu_frame_time_point m_timeStamp;
	};

	bool merge(const IDecodedFrameImpl* i_frame, frame_source_id i_id, const mcu_frame_time_point& i_timeStamp) override;
	void visit(FusionedFrameVisitor& i_visitor) override;

	bool applies_to_scene(frame_source_id i_id, const mcu_frame_time_point& i_timeStamp, size_t& o_pos, bool& isNew);

	void Shift(size_t o_pos, int diff);

	std::map<frame_source_id,frame_info,detail::FrameIdComparator> m_frameInfo;
};

}