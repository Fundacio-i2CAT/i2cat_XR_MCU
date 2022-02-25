
#include "ddk_fiber_impl.h"

namespace ddk
{

template<typename Allocator, typename ... Args>
fiber make_fiber(Args&& ... i_args)
{
	return make_unique_reference<detail::fiber_impl>(make_shared_reference<Allocator>(std::forward<Args>(i_args) ...));
}

}
