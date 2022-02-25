
#include "ddk_macros.h"

namespace ddk
{

template<typename T>
bool fiber_local_storage<T>::empty(const fiber_id& i_id) const
{
	return m_fiberStorage.find(i_id) == m_fiberStorage.end();
}
template<typename T>
template<typename ... Args>
T* fiber_local_storage<T>::construct(const fiber_id& i_id, Args&& ... i_args)
{
	typedef typename std::unordered_map<fiber_id,T>::iterator iterator;

	iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber == m_fiberStorage.end())
	{
		std::pair<iterator,bool> insertRes = m_fiberStorage.emplace(std::make_pair(i_id,std::forward<Args>(i_args) ...));

		return (insertRes.second) ? &(insertRes.first->second) : nullptr;
	}
	else
	{
		DDK_FAIL("Trying to construct already present address");

		return nullptr;
	}
}
template<typename T>
void fiber_local_storage<T>::destroy(const fiber_id& i_id)
{
	typedef typename std::unordered_map<fiber_id,T>::iterator iterator;

	iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber != m_fiberStorage.end())
	{
        m_fiberStorage.erase(itFiber);
	}
	else
	{
		DDK_FAIL("Trying to construct already present address");
	}
}
template<typename T>
template<typename ... Args>
T* fiber_local_storage<T>::assign(const fiber_id& i_id, Args&& ... i_args)
{
	typedef typename std::unordered_map<fiber_id,T>::iterator iterator;

	iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber != m_fiberStorage.end())
	{
	    return &(itFiber->second = T{std::forward<Args>(i_args) ...});
	}
	else
	{
		DDK_FAIL("Trying to construct already present address");

		return nullptr;
	}
}
template<typename T>
T& fiber_local_storage<T>::get(const fiber_id& i_id)
{
	typedef typename std::unordered_map<fiber_id,T>::iterator iterator;

	iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber != m_fiberStorage.end())
	{
		return itFiber->second;
	}
	else
	{
		return crash_on_return<T&>::value();
	}
}
template<typename T>
T fiber_local_storage<T>::extract(const fiber_id& i_id)
{
	typedef typename std::unordered_map<fiber_id,T>::iterator iterator;

	iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber != m_fiberStorage.end())
	{
		T res = std::move(itFiber->second);

		m_fiberStorage.erase(itFiber);

		return std::move(res);
	}
	else
	{
		return crash_on_return<T>::value();
	}
}
template<typename T>
void fiber_local_storage<T>::clear(const fiber_id& i_id)
{
	typename std::unordered_map<fiber_id,T>::iterator itFiber = m_fiberStorage.find(i_id);
	if(itFiber != m_fiberStorage.end())
	{
		m_fiberStorage.erase(itFiber);
	}
}

}
