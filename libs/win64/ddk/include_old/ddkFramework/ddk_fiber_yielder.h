#pragma once

#include "ddk_fiber_scheduler_interface.h"
#include "ddk_thread_yielder.h"

namespace ddk
{
namespace detail
{

struct yielder
{
public:
	explicit yielder(yielder_interface& i_executor);
	inline void yield(yielder_context* i_context)
	{
		m_executor.yield(i_context);
	}
	inline void suspend(yielder_context* i_context)
	{
		m_executor.suspend(i_context);
	}

private:
	yielder_interface& m_executor;
};

}
}