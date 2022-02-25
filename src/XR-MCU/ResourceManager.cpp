#include "ResourceManager.h"
#include "CpuResourceVisitor.h"

namespace mcu
{
namespace detail
{

RegisteredCpuResource::RegisteredCpuResource(bool i_mandatory,cpu_resource_lent_ref i_consumer)
: m_mandatory(i_mandatory)
, m_consumer(i_consumer)
{
}
cpu_resource_lent_ref RegisteredCpuResource::get_resource()
{
	return m_consumer;
}
cpu_resource_const_lent_ref RegisteredCpuResource::get_resource() const
{
	return m_consumer;
}
bool RegisteredCpuResource::is_mandatory() const
{
	return m_mandatory;
}
size_t RegisteredCpuResource::needed_resources() const
{
	return m_consumer->needed_resources();
}
void RegisteredCpuResource::set_affinity(const cpu_set_t& i_cpuSet)
{
	m_consumer->set_affinity(i_cpuSet);
}

}

resource_manager_lent_ref ResourceManager::get_instance()
{
	static ResourceManager s_resourceManager;

	return ddk::lend(s_resourceManager);
}
void ResourceManager::resgiter_resource_consumer(cpu_resource_lent_ref i_resource, bool i_mandatory)
{
	i_resource->sig_onStateChanged.connect(ddk::make_function(this,&ResourceManager::onConsumerStateChanged,i_resource));

	m_resourceConsumers.push_back(ddk::make_unique_reference<detail::RegisteredCpuResource>(i_mandatory,i_resource));
}
void ResourceManager::onConsumerStateChanged(resource_lent_ref i_consumer)
{
	//check for cpu constraints change
	CpuResourceVisitor _visitor;

	for(auto& consumer : m_resourceConsumers)
	{
		consumer->visit(_visitor);
	}

	_visitor.resolve();
}

}