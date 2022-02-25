
#include "ddk_macros.h"

namespace ddk
{

template<typename T>
thread_local_storage<T>::thread_local_storage(thread_local_storage&& other)
: m_address(nullptr)
{
	std::swap(m_address,other.m_address);
}
template<typename T>
thread_local_storage<T>::~thread_local_storage()
{
	if (m_address)
	{
		destroy();
	}
}
template<typename T>
bool thread_local_storage<T>::empty() const
{
	return m_address == nullptr;
}
template<typename T>
template<typename ... Args>
T* thread_local_storage<T>::construct(Args&& ... i_args)
{
	DDK_ASSERT(m_address==nullptr, "Constructing already constructed address");

	if((m_address = m_alloc.allocate(1,sizeof(T))))
	{
		new(m_address) T(std::forward<Args>(i_args) ...);

		return reinterpret_cast<T*>(m_address);
	}
	else
	{
		DDK_FAIL("Could not allocate address");

		return nullptr;
	}
}
template<typename T>
void thread_local_storage<T>::destroy()
{
	DDK_ASSERT(m_address!=nullptr, "Destroying null address");

	if(T* typedAddress = reinterpret_cast<T*>(m_address))
	{
		typedAddress->~T();

		m_alloc.deallocate(m_address);

		m_address = nullptr;
	}
}
template<typename T>
template<typename ... Args>
T* thread_local_storage<T>::assign(Args&& ... i_args)
{
	if (T* typedAddress = reinterpret_cast<T*>(m_address))
	{
		*typedAddress = T(std::forward<Args>(i_args) ...);

		return typedAddress;
	}
	else
	{
		if ((m_address = m_alloc.allocate(1, sizeof(T))))
		{
			new(m_address) T(std::forward<Args>(i_args) ...);

			return reinterpret_cast<T*>(m_address);
		}
		else
		{
			DDK_FAIL("Could not allocate address");

			return nullptr;
		}
	}
}
template<typename T>
T& thread_local_storage<T>::get()
{
	return *reinterpret_cast<T*>(m_address);
}
template<typename T>
T thread_local_storage<T>::extract()
{
	if(T* typedAddress = reinterpret_cast<T*>(m_address))
	{
		T res = std::move(*typedAddress);

		typedAddress->~T();

		m_alloc.deallocate(m_address);

		m_address = nullptr;

		return std::move(res);
	}
	else
	{
		DDK_FAIL("Trying to extract empty thread local storage");

		return crash_on_return<T>::value();
	}
}
template<typename T>
T* thread_local_storage<T>::get_address()
{
	return reinterpret_cast<T*>(m_address);
}

}