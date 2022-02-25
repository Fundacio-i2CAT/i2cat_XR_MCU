#pragma once

#include "ddk_fiber_defs.h"
#include "ddk_thread_yielder.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_function.h"
#include <exception>

namespace ddk
{
namespace detail
{

class yielder_interface
{
protected:
	virtual ~yielder_interface() = default;

public:
	virtual void yield(yielder_context*) = 0;
	virtual void suspend(yielder_context*) = 0;
};

class scheduler_interface
{
public:
	virtual ~scheduler_interface() = default;
	virtual bool activate(fiber_id, const ddk::function<void()>&) = 0;
	virtual bool deactivate(fiber_id) = 0;
	virtual void unregister(fiber_id) = 0;
};

}

typedef shared_reference_wrapper<detail::scheduler_interface> scheduler_interface_shared_ref;
typedef shared_reference_wrapper<const detail::scheduler_interface> scheduler_interface_const_shared_ref;
typedef shared_pointer_wrapper<detail::scheduler_interface> scheduler_interface_shared_ptr;
typedef shared_pointer_wrapper<const detail::scheduler_interface> scheduler_interface_const_shared_ptr;

typedef lent_reference_wrapper<detail::scheduler_interface> scheduler_interface_lent_ref;
typedef lent_reference_wrapper<const detail::scheduler_interface> scheduler_interface_const_lent_ref;
typedef lent_pointer_wrapper<detail::scheduler_interface> scheduler_interface_lent_ptr;
typedef lent_pointer_wrapper<const detail::scheduler_interface> scheduler_interface_const_lent_ptr;

}
