
#include "ddk_fiber_utils.h"

namespace ddk
{

template<typename T, typename Tag>
template<typename ... Args>
T& fiberlocal<T,Tag>::acquire(Args&& ... i_args)
{
	fiber_local_storage<T>& address = get_address();

	if(address.empty(get_current_fiber_id()))
	{
		return *address.construct(get_current_fiber_id(),std::forward<Args>(i_args) ...);
	}
	else
	{
		return address.get(get_current_fiber_id());
	}
}
template<typename T, typename Tag>
template<typename ... Args>
T& fiberlocal<T,Tag>::set(Args&& ... i_args)
{
	fiber_local_storage<T>& address = get_address();

	if (address.empty(get_current_fiber_id()))
	{
		return *address.construct(get_current_fiber_id(),std::forward<Args>(i_args) ...);
	}
	else
	{
		return *address.assign(get_current_fiber_id(),std::forward<Args>(i_args) ...);
	}
}
template<typename T, typename Tag>
T& fiberlocal<T,Tag>::get()
{
	fiber_local_storage<T>& address = get_address();

	return address.get(get_current_fiber_id());
}
template<typename T, typename Tag>
T* fiberlocal<T,Tag>::get_ptr()
{
	fiber_local_storage<T>& address = get_address();

	return address.get_ptr(get_current_fiber_id());
}
template<typename T,typename Tag>
T fiberlocal<T,Tag>::extract()
{
	fiber_local_storage<T>& address = get_address();

	return address.extract(get_current_fiber_id());
}
template<typename T, typename Tag>
void fiberlocal<T,Tag>::clear()
{
	fiber_local_storage<T>& address = get_address();

	address.clear(get_current_fiber_id());
}
template<typename T, typename Tag>
bool fiberlocal<T,Tag>::empty() const
{
	fiber_local_storage<T>& address = get_address();

	return address.empty(get_current_fiber_id());
}
template<typename T, typename Tag>
fiber_local_storage<T>& fiberlocal<T,Tag>::get_address() const
{
	static thread_local fiber_local_storage<T> s_address;

	return s_address;
}

}
