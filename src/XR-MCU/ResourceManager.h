#pragma once

#include "IResourceManager.h"
#include "ddk_lend_from_this.h"
#include <set>

namespace mcu
{
namespace detail
{

class RegisteredCpuResource : public IRegisteredCpuResource
{
public:
	RegisteredCpuResource(bool i_mandatoty, cpu_resource_lent_ref i_consumer);

	cpu_resource_lent_ref get_resource() override;
	cpu_resource_const_lent_ref get_resource() const override;
	bool is_mandatory() const override;
	size_t needed_resources() const override;
	void set_affinity(const cpu_set_t&) override;

private:
	bool m_mandatory;
	cpu_resource_lent_ref m_consumer;
};

}

class ResourceManager : public IResourceManager, public ddk::lend_from_this<ResourceManager,IResourceManager>
{
public:
	static resource_manager_lent_ref get_instance();

private:
	ResourceManager() = default;

	void resgiter_resource_consumer(cpu_resource_lent_ref i_resource, bool i_mandatory) override;
	void onConsumerStateChanged(resource_lent_ref i_consumer);

	std::vector<registered_system_resource_unique_ref> m_resourceConsumers;
};

}