#pragma once

#include "ddk_lend_from_this.h"

namespace ddk
{

class resource_deleter_interface : public lend_from_this<resource_deleter_interface>
{
public:
	virtual ~resource_deleter_interface() = default;
	virtual void deallocate(const void*) const = 0;
};

typedef lent_reference_wrapper<resource_deleter_interface> resource_deleter_lent_ref;
typedef lent_reference_wrapper<const resource_deleter_interface> resource_deleter_const_lent_ref;
typedef lent_pointer_wrapper<resource_deleter_interface> resource_deleter_lent_ptr;
typedef lent_pointer_wrapper<const resource_deleter_interface> resource_deleter_const_lent_ptr;

}