#pragma once

#include "IFusionCollection.h"
#include "ICpuResource.h"
#include "FrameFusionCollector.h"
#include "FrameWriterFactory.h"
#include "FrameEncoderBank.h"
#include "ddk_mutex.h"
#include "ddk_unique_reference_wrapper.h"
#include <map>

namespace mcu
{

class PerPlayerFusionCollection : public IFusionCollection, public ICpuResource
{
public:
	PerPlayerFusionCollection(const IFusionProvider& i_fusionProvider, const McuConfig& i_config);
	~PerPlayerFusionCollection();

	void init(const encoder_context& i_context);

private:
	void add_id(frame_source_id i_id, iphysical_object_const_lent_ref i_obj, const encoder_context& i_context) override;
	void remove_id(frame_source_id i_id) override;
	void set_affinity(const cpu_set_t&) override;
	size_t needed_resources() const override;

	void update_scene(frame_source_id);

	const IFusionProvider& m_fusionProvider; 
	const McuConfig m_config;
	std::map<frame_source_id,frame_fusion_collector_unique_ref> m_frameCollectors;
	ddk::lendable<FrameEncoderBank> m_encoderBank;
	FrameWriterFactory m_frameWriterFactory;
	ddk::mutex m_mutex;
};

}