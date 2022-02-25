
#include "ddk_lock_guard.h"

namespace ddk
{
namespace detail
{

template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(std::nullptr_t)
{
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(T* i_data,const tagged_pointer_deleter& i_refDeleter)
: m_ptr(i_data,i_refDeleter)
{
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(const atomic_shared_pointer_wrapper_impl& other)
{
    lock_guard<spin_lock> lg(other.m_barrier);

    m_ptr = other.m_ptr;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<T,ReferenceCounter>& other)
{
	m_ptr = other;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(atomic_shared_pointer_wrapper_impl&& other)
{
    lock_guard<spin_lock> lg(other.m_barrier);

    m_ptr = std::move(other.m_ptr);
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<T,ReferenceCounter>&& other)
{
	m_ptr = std::move(other);
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(const atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>& other)
{
    lock_guard<spin_lock> lg(other.m_barrier);

    m_ptr = other.m_ptr;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other)
{
	m_ptr = other;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other)
: m_barrier(false)
{
    lock_guard<spin_lock> lg(other.m_barrier);

    m_ptr = std::move(other.m_ptr);
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::atomic_shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other)
{
	m_ptr = std::move(other);
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::~atomic_shared_pointer_wrapper_impl()
{
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(std::nullptr_t)
{
    lock_guard<spin_lock> lg(m_barrier);

    m_ptr = nullptr;

	return *this;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(const atomic_shared_pointer_wrapper_impl& other)
{
    lock_guard<spin_lock,spin_lock> lg(m_barrier,other.m_barrier);

    m_ptr = other.m_ptr;

	return *this;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(const shared_pointer_wrapper_impl<T,ReferenceCounter>& other)
{
    lock_guard<spin_lock> lg(m_barrier);

    m_ptr = other;

    return *this;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(atomic_shared_pointer_wrapper_impl&& other)
{
    lock_guard<spin_lock,spin_lock> lg(m_barrier,other.m_barrier);

    m_ptr = std::move(other.m_ptr);

	return *this;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(shared_pointer_wrapper_impl<T,ReferenceCounter>&& other)
{
    lock_guard<spin_lock> lg(m_barrier);

    m_ptr = std::move(other);

	return *this;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(const atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>& other)
{
    lock_guard<spin_lock,spin_lock> lg(m_barrier,other.m_barrier);

    m_ptr = other.m_ptr;

	return *this;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other)
{
    lock_guard<spin_lock> lg(m_barrier);

    m_ptr = other;

	return *this;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other)
{
    lock_guard<spin_lock,spin_lock> lg(m_barrier,other.m_barrier);

    m_ptr = std::move(other.m_ptr);

	return *this;
}
template<typename T, typename ReferenceCounter>
template<typename TT>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator=(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other)
{
    lock_guard<spin_lock> lg(m_barrier);

    m_ptr = std::move(other);

	return *this;
}
template<typename T,typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator shared_pointer_wrapper_impl<T,ReferenceCounter>&()
{
	return m_ptr;
}
template<typename T,typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator const shared_pointer_wrapper_impl<T,ReferenceCounter>&() const
{
	return m_ptr;
}
template<typename T, typename ReferenceCounter>
bool atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator==(std::nullptr_t) const
{
	return m_ptr == nullptr;
}
template<typename T, typename ReferenceCounter>
bool atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator!=(std::nullptr_t) const
{
	return m_ptr != nullptr;
}
template<typename T, typename ReferenceCounter>
T* atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator->()
{
	return m_ptr.get();
}
template<typename T, typename ReferenceCounter>
const T* atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator->() const
{
	return m_ptr.get();
}
template<typename T, typename ReferenceCounter>
T& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator*()
{
	return *m_ptr;
}
template<typename T, typename ReferenceCounter>
const T& atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator*() const
{
	return *m_ptr;
}
template<typename T, typename ReferenceCounter>
atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator bool() const
{
	return m_ptr != nullptr;
}
template<typename T, typename ReferenceCounter>
void atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::clear()
{
	m_ptr.clear();
}
template<typename T, typename ReferenceCounter>
T* atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::get()
{
	return m_ptr.get();
}
template<typename T, typename ReferenceCounter>
const T* atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::get() const
{
	return m_ptr.get();
}
template<typename T, typename ReferenceCounter>
tagged_pointer_deleter atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::get_deleter() const
{
	return m_ptr.get_deleter();
}
template<typename T, typename ReferenceCounter>
bool atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::empty() const
{
	return m_ptr.empty();
}

}
}
