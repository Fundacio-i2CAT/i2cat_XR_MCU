#pragma once

#include "ISystemResource.h"
#include <sched.h>

namespace mcu
{

class ICpuResource : public ISystemResource
{
public:
	virtual void set_affinity(const cpu_set_t&) = 0;
};

typedef ddk::lent_reference_wrapper<ICpuResource> cpu_resource_lent_ref;
typedef ddk::lent_reference_wrapper<const ICpuResource> cpu_resource_const_lent_ref;

namespace detail
{

class IRegisteredCpuResource : public IRegisteredSystemResource
{
public:
	virtual cpu_resource_lent_ref get_resource() = 0;
	virtual cpu_resource_const_lent_ref get_resource() const = 0;
	virtual void set_affinity(const cpu_set_t&) = 0;

private:
	void visit(const SystemResourceVisitor&) const override;
	void visit(SystemResourceVisitor&) const override;
};

}

typedef ddk::lent_reference_wrapper<detail::IRegisteredCpuResource> registered_cpu_resource_lent_ref;
typedef ddk::lent_reference_wrapper<const detail::IRegisteredCpuResource> registered_cpu_resource_const_lent_ref;

}