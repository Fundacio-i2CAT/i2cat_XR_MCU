#pragma once

#include "IFusionedFrameEncoder.h"
#include "EncodedFrame.h"
#include "DecodedFrame.h"
#include "FrameEncoderCache.h"
#include "ddk_result.h"
#include "ddk_task_executor.h"

namespace mcu
{

class FrameEncoderBase : public IFusionedFrameEncoder
{
public:
	FrameEncoderBase(const frame_dest_id& i_id, ddk::task_executor_lent_ref i_taskExecutor, size_t i_maxNumPendingDecodedFrames);

protected:
	enum class EncodeFrameError
	{
		DecoderNotAvailable,
		DecodedFrameNotEncodable
	};

private:
	enum class State
	{
		Idle,
		Running,
		Stop
	};

	void start() override;
	void stop() override;
	ddk::shared_future<encoded_frame_dist_ref> encode_frame(const ddk::function<FusionedFrame(const frame_souce_time_stamp_list&)>& i_frameFusioner, const frame_souce_time_stamp_list&) override;

	virtual encoded_frame_dist_ref processFrameData(const ddk::function<FusionedFrame(const frame_souce_time_stamp_list&)>&,const frame_souce_time_stamp_list&) = 0;

	const frame_dest_id m_id;
	ddk::task_executor_lent_ref m_taskExecutor;
	const size_t m_maxNumPendingDecodedFrames;
	ddk::single_consumer_lock_free_stack<FusionedFrame> m_pendingFramesStack;
	ddk::atomic_size_t m_numPendingDecodedFrames;
	State m_state;
	FrameEncoderCache m_cache;
};

}