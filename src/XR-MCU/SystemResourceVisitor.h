#pragma once

#include "ICpuResource.h"

namespace mcu
{

class SystemResourceVisitor
{
public:
	virtual void operator()(const detail::IRegisteredSystemResource& i_resource);
	virtual void operator()(const detail::IRegisteredSystemResource& i_resource) const;
	virtual void operator()(const detail::IRegisteredCpuResource& i_resource);
	virtual void operator()(const detail::IRegisteredCpuResource& i_resource) const;
};

}