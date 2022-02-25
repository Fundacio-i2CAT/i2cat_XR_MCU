#pragma once

#include "ddk_spin_lock.h"
#include "ddk_weak_pointer_wrapper.h"
#include "ddk_atomic_shared_pointer_wrapper.h"

namespace ddk
{

template<typename T>
class atomic_weak_pointer_wrapper
{
public:
	atomic_weak_pointer_wrapper() = default;
	atomic_weak_pointer_wrapper(const atomic_weak_pointer_wrapper& other);
	atomic_weak_pointer_wrapper(const weak_pointer_wrapper<T>& other);
	atomic_weak_pointer_wrapper(atomic_weak_pointer_wrapper&& other);
	atomic_weak_pointer_wrapper(weak_pointer_wrapper<T>&& other);
	template<typename TT>
	atomic_weak_pointer_wrapper(const atomic_weak_pointer_wrapper<TT>& other);
	template<typename TT>
	atomic_weak_pointer_wrapper(const weak_pointer_wrapper<TT>& other);
	template<typename TT>
	atomic_weak_pointer_wrapper(atomic_weak_pointer_wrapper<TT>&& other);
	template<typename TT>
	atomic_weak_pointer_wrapper(weak_pointer_wrapper<TT>&& other);
	~atomic_weak_pointer_wrapper();

	atomic_weak_pointer_wrapper& operator=(std::nullptr_t);
	atomic_weak_pointer_wrapper& operator=(const atomic_weak_pointer_wrapper& other);
	atomic_weak_pointer_wrapper& operator=(const weak_pointer_wrapper<T>& other);
	atomic_weak_pointer_wrapper& operator=(atomic_weak_pointer_wrapper&& other);
	atomic_weak_pointer_wrapper& operator=(weak_pointer_wrapper<T>&& other);
	template<typename TT>
	atomic_weak_pointer_wrapper& operator=(const atomic_weak_pointer_wrapper<TT>& other);
	template<typename TT>
	atomic_weak_pointer_wrapper& operator=(const weak_pointer_wrapper<TT>& other);
	template<typename TT>
	atomic_weak_pointer_wrapper& operator=(atomic_weak_pointer_wrapper<TT>&& other);
	template<typename TT>
	atomic_weak_pointer_wrapper& operator=(weak_pointer_wrapper<TT>&& other);

	bool operator==(const std::nullptr_t&) const;
	bool operator!=(const std::nullptr_t&) const;
	operator weak_pointer_wrapper<T>() const;
	atomic_shared_pointer_wrapper<T> share() const;

private:
	mutable spin_lock m_barrier;
	weak_pointer_wrapper<T> m_ptr;
};

}

#include "ddk_atomic_weak_pointer_wrapper.inl"