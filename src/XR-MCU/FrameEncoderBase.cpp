#include "FrameEncoderBase.h"
#include "FusionModuleProfiler.h"

namespace mcu
{

FrameEncoderBase::FrameEncoderBase(const frame_dest_id& i_id, ddk::task_executor_lent_ref i_taskExecutor, size_t i_maxNumPendingDecodedFrames)
: m_id(i_id)
, m_taskExecutor(i_taskExecutor)
, m_maxNumPendingDecodedFrames(i_maxNumPendingDecodedFrames)
, m_state(State::Idle)
, m_numPendingDecodedFrames(0)
{
}
void FrameEncoderBase::start()
{
	m_state = State::Running;
}
void FrameEncoderBase::stop()
{
	m_state = State::Stop;
}
ddk::shared_future<encoded_frame_dist_ref> FrameEncoderBase::encode_frame(const ddk::function<FusionedFrame(const frame_souce_time_stamp_list&)>& i_frameFusioner, const frame_souce_time_stamp_list& i_sceneInfo)
{
	if(ddk::shared_future<encoded_frame_dist_ref> cachedFrame = m_cache.get(i_sceneInfo))
	{
		return cachedFrame;
	}
	else
	{
		ddk::shared_future<encoded_frame_dist_ref> encodingFuture = m_taskExecutor->enqueue(ddk::make_function(this,&FrameEncoderBase::processFrameData,i_frameFusioner,i_sceneInfo));

		m_cache.set(i_sceneInfo,encodingFuture);

		return encodingFuture;
	}
}

}