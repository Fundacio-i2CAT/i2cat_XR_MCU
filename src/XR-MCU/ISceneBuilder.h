#pragma once

#include "FrameDefs.h"
#include "PlayerTransformation.h"
#include "ddk_lent_reference_wrapper.h"
#include "EncodedFrame.h"
#include "FusionedFrame.h"
#include "ddk_lend_from_this.h"
#include "ddk_future.h"

namespace mcu
{

class ISceneBuilder : public ddk::lend_from_this<ISceneBuilder>
{
public:
	ISceneBuilder() = default;
	ISceneBuilder(const ISceneBuilder&) = default;
	ISceneBuilder(ISceneBuilder&&) = default;
	virtual ~ISceneBuilder() = default;
	virtual bool add_source(frame_source_id i_id) = 0;
	virtual bool remove_source(frame_source_id i_id) = 0;
	virtual size_t size() const = 0;
	virtual bool empty() const = 0;
	virtual const transformation& get_transform() const = 0;
	virtual Frustrum get_frustrum() const = 0;

	ddk::function<ddk::shared_future<encoded_frame_dist_ref>(const ddk::function<FusionedFrame(const frame_souce_time_stamp_list&)>&,const frame_souce_time_stamp_list&)> call_onFrameToEncode;
};

typedef ddk::lent_reference_wrapper<ISceneBuilder> scene_builder_lent_ref;
typedef ddk::lent_reference_wrapper<const ISceneBuilder> scene_builder_const_lent_ref;
typedef ddk::lent_pointer_wrapper<ISceneBuilder> scene_builder_lent_ptr;
typedef ddk::lent_pointer_wrapper<const ISceneBuilder> scene_builder_const_lent_ptr;

}