#include "PerPlayerFusionCollection.h"
#include "ResourceManager.h"
#include "B2DFrameWriter.h"
#include "MCUManagerDefs.h"
#include "ddk_reference_wrapper.h"
#include "ddk_lock_guard.h"
#include <algorithm>

namespace mcu
{

PerPlayerFusionCollection::PerPlayerFusionCollection(const IFusionProvider& i_fusionProvider, const McuConfig& i_config)
: m_fusionProvider(i_fusionProvider)
, m_config(i_config)
, m_mutex(ddk::MutexType::Recursive)
{
	m_fusionProvider.sig_onBoundingBoxChanged.connect(ddk::make_function(this,&PerPlayerFusionCollection::update_scene));
}
PerPlayerFusionCollection::~PerPlayerFusionCollection()
{
	std::map<frame_source_id,frame_fusion_collector_unique_ref>::iterator itCollector = m_frameCollectors.begin();
	for (; itCollector != m_frameCollectors.end(); ++itCollector)
	{
		itCollector->second->stop();
	}
}
void PerPlayerFusionCollection::init(const encoder_context& i_context)
{
	m_encoderBank->init(i_context);

	ResourceManager::get_instance()->resgiter_resource_consumer(ddk::lend(m_encoderBank));
}
void PerPlayerFusionCollection::add_id(frame_source_id i_id, iphysical_object_const_lent_ref i_obj, const encoder_context& i_context)
{
	typedef std::map<frame_source_id,frame_fusion_collector_unique_ref>::iterator iterator;

	m_mutex.lock();

	iterator itCollector = m_frameCollectors.lower_bound(i_id);
	if (itCollector == m_frameCollectors.end() || itCollector->first != i_id)
	{
		try
		{
			const frame_dest_id destId(i_id.getValue());

			iterator itInserted = m_frameCollectors.insert(itCollector,std::make_pair(i_id,ddk::make_unique_reference<FrameFusionCollector>(destId,i_obj,m_fusionProvider,m_frameWriterFactory.create_frame_writer(destId,i_context),m_config)));

			if (itInserted->first == i_id)
			{
				frame_fusion_collector_lent_ref insertedCollector = ddk::lend(itInserted->second);
				FrameEncoderBank::register_result regRes = m_encoderBank->register_frame_dest(destId,i_context.m_codec,insertedCollector);

				if (regRes)
				{
					insertedCollector->start();

					m_mutex.unlock();

					i_obj->sig_onUpdate.connect([i_id,i_obj,this](){ this->update_scene(i_id); });

					sig_onStateChanged.execute();
				}
				else
				{
					DDK_FAIL("Error registering encoder");
				}
			}
			else
			{
				DDK_FAIL("Error creating frame collector");
			}
		}
		catch (const FusionSenderException&)
		{
			m_mutex.lock();

			throw;
		}
	}

	m_mutex.unlock();
}
void PerPlayerFusionCollection::remove_id(frame_source_id i_id)
{
	typedef std::map<frame_source_id,frame_fusion_collector_unique_ref>::iterator iterator;

	ddk::mutex_guard mg(m_mutex);

	iterator itCollector = m_frameCollectors.lower_bound(i_id);
	if(itCollector != m_frameCollectors.end())
	{
		m_frameCollectors.erase(itCollector);
	}
}
void PerPlayerFusionCollection::set_affinity(const cpu_set_t& i_cpuSet)
{
	ddk::mutex_guard mg(m_mutex);

	std::map<frame_source_id,frame_fusion_collector_unique_ref>::iterator itCollector = m_frameCollectors.begin();
	for(; itCollector != m_frameCollectors.end(); ++itCollector)
	{
		itCollector->second->set_affinity(i_cpuSet);
	}
}
size_t PerPlayerFusionCollection::needed_resources() const
{
	return k_numCpuForFusion;
}
void PerPlayerFusionCollection::update_scene(frame_source_id i_id)
{
	bool somethingChanged = false;

	{
		ddk::mutex_guard mg(m_mutex);

		std::map<frame_source_id,frame_fusion_collector_unique_ref>::iterator itCollector = m_frameCollectors.begin();
		std::map<frame_source_id,frame_fusion_collector_unique_ref>::iterator itUpdatedCollector = m_frameCollectors.find(i_id);
		scene_builder_lent_ref updatedScene = ddk::lend(itUpdatedCollector->second);
		const Frustrum updateFrustrum = updatedScene->get_frustrum();
		const transformation& updateTransform = updatedScene->get_transform();
		const bounding_box updatedBoundingBox = m_fusionProvider.get_bounding_box(i_id);
		for (; itCollector != m_frameCollectors.end() && itUpdatedCollector != m_frameCollectors.end(); ++itCollector)
		{
			if (i_id != itCollector->first)
			{
				scene_builder_lent_ref currScene = ddk::lend(itCollector->second);

				const bounding_box collectoBoundingBox = m_fusionProvider.get_bounding_box(itCollector->first);
				const Frustrum::Area updateOverlap = updateFrustrum & collectoBoundingBox;

				if (updateOverlap != Frustrum::None)
				{
					somethingChanged = updatedScene->add_source(itCollector->first);
				}
				else
				{
					//no intersection, remove it from idList
					somethingChanged = updatedScene->remove_source(itCollector->first);
				}

				const Frustrum collectorFrustrum = currScene->get_frustrum();
				const Frustrum::Area collectorOverlap = collectorFrustrum & updatedBoundingBox;

				if(collectorOverlap != Frustrum::None)
				{
					somethingChanged = currScene->add_source(i_id);
				}
				else
				{
					//no intersection, remove it from idList
					somethingChanged = currScene->remove_source(i_id);
				}
			}
		}
	}

	if(somethingChanged)
	{
		m_encoderBank->notify_resource_change();
	}
}

}