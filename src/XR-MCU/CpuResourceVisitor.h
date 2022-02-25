#pragma once

#include "SystemResourceVisitor.h"
#include "ddk_function.h"

namespace mcu
{

class CpuResourceVisitor : public SystemResourceVisitor
{
public:
	CpuResourceVisitor() = default;

	void resolve();

private:
	void operator()(const detail::IRegisteredCpuResource& i_resource) override;

	std::vector<cpu_resource_lent_ref> m_mandatoryConsumers;
	std::vector<cpu_resource_lent_ref> m_consumers;
	size_t m_totalQuery = 0;
};

}