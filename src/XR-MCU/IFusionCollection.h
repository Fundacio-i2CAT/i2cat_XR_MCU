#pragma once

#include "FrameDefs.h"
#include "FrameEncoderDefs.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "FusionDefs.h"
#include "IPhysicalObject.h"

namespace mcu
{

class IFusionCollection
{
public:
	virtual ~IFusionCollection() = default;

	virtual void add_id(frame_source_id, iphysical_object_const_lent_ref, const encoder_context&) = 0;
	virtual void remove_id(frame_source_id) = 0;
};

typedef ddk::unique_pointer_wrapper<IFusionCollection> ifusion_collection_unique_ptr;
typedef ddk::unique_pointer_wrapper<const IFusionCollection> ifusion_collection_const_unique_ptr;

}