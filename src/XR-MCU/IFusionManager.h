#pragma once

#include "ddk_unique_reference_wrapper.h"
#include "IDecodedFrameReceiver.h"
#include "FrameEncoderDefs.h"
#include "ProfilerDefs.h"
#include "IPhysicalObject.h"

namespace mcu
{

class IFusionManager : public IDecodedFrameReceiver
{
public:
	enum RegisterErrorCode
	{
		SourceAlreadyRegistered,
		InvalidFrameEncoderFormat,
		InvalidFrameWriterFormat
	};
	typedef ddk::result<void,RegisterErrorCode> register_result;
	enum UnregisterErrorCode
	{
		SourceNotRegistered
	};
	typedef ddk::result<void,UnregisterErrorCode> unregister_result;

	virtual ~IFusionManager() = default;

	virtual void start() = 0;
	virtual register_result register_frame_source_id(frame_source_id,iphysical_object_const_lent_ref, const encoder_context&) = 0;
	virtual unregister_result unregister_frame_source_id(frame_source_id) = 0;
};

typedef ddk::unique_reference_wrapper<IFusionManager> fusion_manager_unique_ref;
typedef ddk::unique_reference_wrapper<const  IFusionManager> fusion_manager_const_unique_ref;
typedef ddk::unique_pointer_wrapper<IFusionManager> fusion_manager_unique_ptr;
typedef ddk::unique_pointer_wrapper<const  IFusionManager> fusion_manager_const_unique_ptr;

}