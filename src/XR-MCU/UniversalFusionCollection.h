#pragma once

#include "IFusionCollection.h"
#include "ICpuResource.h"
#include "FrameFusionCollector.h"
#include "IFusionProvider.h"
#include "MCUConfig.h"
#include "FrameEncoderBank.h"
#include "FrameWriterFactory.h"

namespace mcu
{

class UniversalFusionCollection : public IFusionCollection, public ICpuResource
{
public:
	UniversalFusionCollection(const IFusionProvider& i_fusionProvider, const McuConfig& i_config);
	~UniversalFusionCollection();

	void init(const encoder_context& i_context);

private:
	void add_id(frame_source_id i_id, iphysical_object_const_lent_ref i_obj, const encoder_context& i_context) override;
	void remove_id(frame_source_id i_id) override;
	void set_affinity(const cpu_set_t&) override;
	size_t needed_resources() const override;

	const IFusionProvider& m_fusionProvider;
	const McuConfig m_config;
	ddk::unique_pointer_wrapper<FrameFusionCollector> m_collector;
	FrameEncoderBank m_encoderBank;
	FrameWriterFactory m_frameWriterFactory;
};


}