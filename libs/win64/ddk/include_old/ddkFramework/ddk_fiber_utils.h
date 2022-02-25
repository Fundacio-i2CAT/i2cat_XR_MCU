#pragma once

#include "ddk_fiber_defs.h"

#define set_current_execution_context(_CONTEXT) \
	ddk::detail::__s_current_execution_context = &(_CONTEXT);

#define get_current_execution_context() \
	*ddk::detail::__s_current_execution_context

namespace ddk
{
namespace detail
{

struct execution_context;

extern thread_local execution_context* __s_current_execution_context;

}

fiber_id get_current_fiber_id();

void suspend();

void yield();

template<typename T>
inline void yield(T&& i_value);

}

#include "ddk_fiber_utils.inl"
