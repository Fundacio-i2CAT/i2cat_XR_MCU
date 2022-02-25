#include "UniversalFusionCollection.h"
#include "B2DFrameWriter.h"
#include "MCUManagerDefs.h"

namespace mcu
{

UniversalFusionCollection::UniversalFusionCollection(const IFusionProvider& i_fusionProvider, const McuConfig& i_config)
: m_fusionProvider(i_fusionProvider)
, m_config(i_config)
{
}
UniversalFusionCollection::~UniversalFusionCollection()
{
	m_collector->stop();
}
void UniversalFusionCollection::init(const encoder_context& i_context)
{
	try
	{
		const frame_dest_id destId("universal");

		m_collector = ddk::make_unique_reference<FrameFusionCollector>(destId,nullptr,m_fusionProvider,m_frameWriterFactory.create_frame_writer(destId,i_context),m_config);

		m_encoderBank.init(i_context);

		FrameEncoderBank::register_result regRes = m_encoderBank.register_frame_dest(frame_dest_id("universal"),i_context.m_codec,ddk::promote_to_ref(ddk::lend(m_collector)));

		if(regRes)
		{
			m_collector->start();
		}
		else
		{
			throw FusionEncoderException{};
		}
	}
	catch(const FusionCreationException&)
	{
		DDK_LOG_ERROR("Error creating frame source in frame collector");

		throw;
	}
}
void UniversalFusionCollection::add_id(frame_source_id i_id, iphysical_object_const_lent_ref i_obj, const encoder_context& i_context)
{
	if(scene_builder_lent_ptr collector = ddk::lend(m_collector))
	{
		collector->add_source(i_id);
	}
}
void UniversalFusionCollection::remove_id(frame_source_id i_id)
{
	if(scene_builder_lent_ptr collector = ddk::lend(m_collector))
	{
		collector->remove_source(i_id);
	}
}
void UniversalFusionCollection::set_affinity(const cpu_set_t& i_cpuSet)
{
	m_collector->set_affinity(i_cpuSet);
}
size_t UniversalFusionCollection::needed_resources() const
{
	return k_numCpuForFusion;
}

}