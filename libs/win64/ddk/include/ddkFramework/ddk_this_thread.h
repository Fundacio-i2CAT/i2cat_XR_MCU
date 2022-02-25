#pragma once

#include "ddk_execution_context.h"

namespace ddk
{
namespace detail
{

struct this_thread_t
{
public:
	this_thread_t();

private:
	execution_context m_execContext;
};

}

const detail::this_thread_t& this_thread();

}
