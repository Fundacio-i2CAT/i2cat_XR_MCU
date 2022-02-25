#pragma once

#include "IFusionProvider.h"
#include "IFusionManager.h"
#include "ddk_exclusion_area.h"
#include "ddk_result.h"
#include "IPhysicalObject.h"

namespace mcu
{

class FusionContainer : public IFusionProvider
{
public:
	FusionContainer(size_t i_maxPendingFrames);

	bool register_frame_source(frame_source_id i_id, iphysical_object_const_lent_ref i_object);
	bool unregister_frame_source(frame_source_id i_id);
	void add_decoded_frame(frame_source_id i_id, DecodedFrame i_frame);

private:
	scene_info get_current_scene_info(const frame_source_list_id&) const override;
	frame_collection get_scene(const scene_info& i_sceneInfo) const override;
	frame_collection FusionContainer::get_last_frame(const frame_source_list_id&) const override;
	bounding_box get_bounding_box(const frame_source_id&) const override;
	size_t get_num_frame_sources() const override;
	critical_context enterReader(ddk::Reentrancy) const override;
	critical_context tryToEnterReader(ddk::Reentrancy) const override;
	void leaveReader(critical_context) const override;
	critical_context enterWriter(ddk::Reentrancy) override;
	critical_context tryToEnterWriter(ddk::Reentrancy) override;
	void leaveWriter(critical_context) override;
	void onNewAvailableFrame() const override;

	const size_t m_maxPendingFrames;
	fusion_container m_fusionCtr;
	std::map<frame_source_id,bounding_box> m_boundingBox;
	mutable ddk::exclusion_area m_exclArea;
};

}