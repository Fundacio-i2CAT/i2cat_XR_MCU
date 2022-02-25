
#include "ddk_lock_guard.h"

namespace ddk
{

template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const atomic_weak_pointer_wrapper& other)
{
    lock_guard<spin_lock> lg(other.m_barrier);

    m_ptr = other.m_ptr;
}
template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const weak_pointer_wrapper<T>& other)
{
	m_ptr = other;
}
template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(atomic_weak_pointer_wrapper&& other)
{
    lock_guard<spin_lock> lg(other.m_barrier);

    m_ptr = std::move(other.m_ptr);
}
template<typename T>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(weak_pointer_wrapper<T>&& other)
{
	m_ptr = std::move(other);
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const atomic_weak_pointer_wrapper<TT>& other)
{
    lock_guard<spin_lock> lg(other.m_barrier);

    m_ptr = other.m_ptr;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(const weak_pointer_wrapper<TT>& other)
{
	m_ptr = other.m_ptr;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(atomic_weak_pointer_wrapper<TT>&& other)
{
    lock_guard<spin_lock> lg(other.m_barrier);

    m_ptr = std::move(other.m_ptr);
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>::atomic_weak_pointer_wrapper(weak_pointer_wrapper<TT>&& other)
{
	m_ptr = std::move(other);
}
template<typename T>
atomic_weak_pointer_wrapper<T>::~atomic_weak_pointer_wrapper()
{
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(std::nullptr_t)
{
    lock_guard<spin_lock> lg(m_barrier);

    m_ptr = nullptr;

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const atomic_weak_pointer_wrapper& other)
{
    lock_guard<spin_lock,spin_lock> lg(m_barrier,other.m_barrier);

    m_ptr = other.m_ptr;

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const weak_pointer_wrapper<T>& other)
{
    lock_guard<spin_lock> lg(m_barrier);

    m_ptr = other;

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(atomic_weak_pointer_wrapper&& other)
{
    lock_guard<spin_lock,spin_lock> lg(m_barrier,other.m_barrier);

    m_ptr = std::move(other.m_ptr);

	return *this;
}
template<typename T>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(weak_pointer_wrapper<T>&& other)
{
    lock_guard<spin_lock> lg(m_barrier);

    m_ptr = std::move(other);

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const atomic_weak_pointer_wrapper<TT>& other)
{
    lock_guard<spin_lock,spin_lock> lg(m_barrier,other.m_barrier);

    m_ptr = other.m_ptr;

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(const weak_pointer_wrapper<TT>& other)
{
    lock_guard<spin_lock> lg(m_barrier);

    m_ptr = other;

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(atomic_weak_pointer_wrapper<TT>&& other)
{
    lock_guard<spin_lock,spin_lock> lg(m_barrier,other.m_barrier);

    m_ptr = std::move(other.m_ptr);

	return *this;
}
template<typename T>
template<typename TT>
atomic_weak_pointer_wrapper<T>& atomic_weak_pointer_wrapper<T>::operator=(weak_pointer_wrapper<TT>&& other)
{
    lock_guard<spin_lock> lg(m_barrier);

    m_ptr = std::move(other);

	return *this;
}
template<typename T>
bool atomic_weak_pointer_wrapper<T>::operator==(const std::nullptr_t&) const
{
	return m_ptr == nullptr;
}
template<typename T>
bool atomic_weak_pointer_wrapper<T>::operator!=(const std::nullptr_t&) const
{
	return m_ptr != nullptr;
}
template<typename T>
atomic_weak_pointer_wrapper<T>::operator weak_pointer_wrapper<T>() const
{
	return m_ptr;
}
template<typename T>
atomic_shared_pointer_wrapper<T> atomic_weak_pointer_wrapper<T>::share() const
{
    lock_guard<spin_lock> lg(m_barrier);

    return atomic_shared_pointer_wrapper<T>{ m_ptr.share() };
}

}
