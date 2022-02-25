#pragma once

#include "ICpuResource.h"
#include "ddk_lent_reference_wrapper.h"

namespace mcu
{

class IResourceManager
{
public:
	virtual ~IResourceManager() = default;

	virtual void resgiter_resource_consumer(cpu_resource_lent_ref, bool i_mandatory = false) = 0;
};

typedef ddk::lent_reference_wrapper<IResourceManager> resource_manager_lent_ref;
typedef ddk::lent_reference_wrapper<IResourceManager> resource_manager_const_lent_ref;

}