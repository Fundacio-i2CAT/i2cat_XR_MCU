
#include <thread>
#include "ddk_reference_wrapper.h"
#include "ddk_thread_impl.h"
#include "ddk_async_exceptions.h"

extern "C"
{

void consolidate_frame();

}

namespace ddk
{
namespace detail
{

template<typename T>
typed_yielder_context<T>* this_fiber_t::get_typed_context() const
{
	return (m_execContext) ? m_execContext->get_typed_context<typed_yielder_context<T>>() : nullptr;
}


template<typename Return>
inline void launch_fiber(const ddk::function<Return()>* i_function, fiber_impl* i_fiber)
{
	if(i_fiber->get_state() == FiberExecutionState::Executing)
	{
		const ddk::function<Return()> localCallable = *i_function;

		try
		{
			eval(localCallable);
		}
		catch(const suspend_exception&)
		{
		}
		catch(const async_exception& i_excp)
		{
			yield(i_excp);
		}
		catch(const std::exception& i_excp)
		{
			yield(async_exception{ i_excp.what() });
		}
		catch(...)
		{
			yield(async_exception{ "Unkwon exception" });
		}
	}

	i_fiber->set_state(FiberExecutionState::Done);
}

template<typename Return>
void fiber_impl::start_from(this_fiber_t& other, const ddk::function<Return()>& i_function)
{
	m_state = FiberExecutionState::Executing;

	other.attach_context();

	m_fiberContext.attach_stack(m_alloc.allocate());

	make_execution_context(m_fiberContext,other.get_execution_context(),&consolidate_frame,&i_function,this,&launch_fiber<Return>);
}

}
}
