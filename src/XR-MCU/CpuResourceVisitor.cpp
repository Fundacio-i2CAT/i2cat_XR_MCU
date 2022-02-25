#include "CpuResourceVisitor.h"
#include "MCUManagerDefs.h"

namespace mcu
{

void CpuResourceVisitor::operator()(const detail::IRegisteredCpuResource& i_resource)
{
	if(i_resource.is_mandatory()) 
	{
		m_mandatoryConsumers.push_back(ddk::static_lent_cast<ICpuResource>(const_cast<detail::IRegisteredCpuResource&>(i_resource).get_resource()));
	}
	else
	{
		m_consumers.push_back(ddk::static_lent_cast<ICpuResource>(const_cast<detail::IRegisteredCpuResource&>(i_resource).get_resource()));
		m_totalQuery += i_resource.needed_resources();
	}

}
void CpuResourceVisitor::resolve()
{
	int lastUsedCpu = 0;
	int numCpus = static_cast<int>(k_numCpus);

	auto sendCpuSet = [&lastUsedCpu](cpu_resource_lent_ref i_consumer, size_t i_numCpus)
	{
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		for(int cpuIndex = 0; cpuIndex < i_numCpus; ++cpuIndex)
		{
			CPU_SET(lastUsedCpu + cpuIndex,&cpuset);
		}

		i_consumer->set_affinity(cpuset);
		lastUsedCpu += static_cast<int>(i_numCpus);
	};

	for(auto& consumer : m_mandatoryConsumers)
	{
		const size_t currConsumerNeeds = consumer->needed_resources();

		numCpus -= static_cast<int>(currConsumerNeeds);

		sendCpuSet(consumer,currConsumerNeeds);
	}

	for(auto& consumer : m_consumers)
	{
		const size_t currConsumerNeeds = consumer->needed_resources();

		const int consumerCpus = (m_totalQuery >= numCpus) ? static_cast<int>((static_cast<float>(currConsumerNeeds) / m_totalQuery) * numCpus) : static_cast<int>(currConsumerNeeds);

		sendCpuSet(consumer,consumerCpus);
	}
}

}