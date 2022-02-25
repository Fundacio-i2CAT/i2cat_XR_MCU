
#include "ddk_thread_yielder.h"
#include "ddk_thread_impl.h"
#include "ddk_reference_wrapper.h"
#include "ddk_async_exceptions.h"

namespace ddk
{

template<typename T>
void yield(T&& i_value)
{
	ddk::detail::execution_context& currFiberContext = get_current_execution_context();

	if(ddk::detail::yielder_interface* currYielder = currFiberContext.get_yielder())
	{
		detail::typed_yielder_context<T> _yielder(std::forward<T>(i_value));

		currYielder->yield(&_yielder);

		if (_yielder.is_stopped(currFiberContext.get_id()))
		{
			throw suspend_exception{ currFiberContext.get_id() };
		}
	}
	else
	{
		std::this_thread::yield();
	}
}

}
