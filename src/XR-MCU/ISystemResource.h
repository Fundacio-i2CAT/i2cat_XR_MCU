#pragma once

#include "ddk_signal.h"

namespace mcu
{

class SystemResourceVisitor;

class ISystemResource
{
public:
	virtual ~ISystemResource() = default;
	virtual size_t needed_resources() const = 0;

	ddk::signal<void()> sig_onStateChanged;
};

typedef ddk::lent_reference_wrapper<ISystemResource> resource_lent_ref;
typedef ddk::lent_reference_wrapper<const ISystemResource> resource_const_lent_ref;

namespace detail
{

class IRegisteredSystemResource
{
public:
	virtual ~IRegisteredSystemResource() = default;
	virtual size_t needed_resources() const = 0;
	virtual bool is_mandatory() const = 0;

	virtual void visit(const SystemResourceVisitor&) const;
	virtual void visit(SystemResourceVisitor&) const;
};

}

typedef ddk::unique_reference_wrapper<detail::IRegisteredSystemResource> registered_system_resource_unique_ref;
typedef ddk::unique_reference_wrapper<const detail::IRegisteredSystemResource> registered_system_resource_const_unique_ref;

}