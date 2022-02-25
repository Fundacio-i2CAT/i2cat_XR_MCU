#pragma once

#include "IFusionedFrameEncoder.h"
#include "FrameEncoderDefs.h"
#include "FrameFusionCollector.h"
#include "FrameEncoderFactory.h"
#include "ICpuResource.h"
#include "ddk_unique_pointer_wrapper.h"
#include "ddk_task_executor.h"
#include <functional>

namespace mcu
{

class FrameEncoderBank : public ICpuResource
{
public:
	enum class RegisterErrorCode
	{
		RegisterCollectorAlreadyExist,
		RegisterUnsupportedEncoder,
		RegisterLackOfResources,
		RegisterInternalError
	};
	typedef ddk::result<void,RegisterErrorCode> register_result;
	enum class UnregisterErrorCode
	{
		UnregisterCollectorNonExist
	};
	typedef ddk::result<void,UnregisterErrorCode> unregister_result;

	FrameEncoderBank();
	~FrameEncoderBank();

	void init(const encoder_context& i_context);

	register_result register_frame_dest(frame_dest_id i_id, EncodecType i_codecType, scene_builder_lent_ref i_collector);
	unregister_result unregister_frame_dest(frame_dest_id i_id);
	void notify_resource_change();

private:
	size_t needed_resources() const override;
	void set_affinity(const cpu_set_t&) override;

	void checkEncoderFrameRate(const frame_sent_fps& i_measure);

	std::map<EncodecType,ifusioned_frame_encoder_unique_ref> m_encoders;
	std::map<frame_dest_id,scene_builder_lent_ref> m_sceneBuilders;
	ddk::task_executor_unique_ptr m_taskExecutor;
	FrameEncoderFactory m_encoderFactory;
	std::vector<ddk::connection> m_measuremntConn;
	mutable pthread_mutex_t m_mutex;
	size_t m_maxPendingDecodedFrames = 0;
};

}