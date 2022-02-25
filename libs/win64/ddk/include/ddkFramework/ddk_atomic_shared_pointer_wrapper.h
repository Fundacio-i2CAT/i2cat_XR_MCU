#pragma once

#include "ddk_shared_pointer_wrapper.h"
#include "ddk_spin_lock.h"
#include "ddk_concepts.h"

namespace ddk
{

namespace detail
{

template<typename T, typename ReferenceCounter>
class atomic_shared_pointer_wrapper_impl
{
public:
	atomic_shared_pointer_wrapper_impl() = default;
	atomic_shared_pointer_wrapper_impl(std::nullptr_t);
	atomic_shared_pointer_wrapper_impl(T* i_data,const tagged_pointer_deleter& i_refDeleter);
	atomic_shared_pointer_wrapper_impl(const atomic_shared_pointer_wrapper_impl& other);
	atomic_shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<T,ReferenceCounter>& other);
	atomic_shared_pointer_wrapper_impl(atomic_shared_pointer_wrapper_impl&& other);
	atomic_shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<T,ReferenceCounter>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl(const atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl(atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other);
	~atomic_shared_pointer_wrapper_impl();
	atomic_shared_pointer_wrapper_impl& operator=(std::nullptr_t);
	atomic_shared_pointer_wrapper_impl& operator=(const atomic_shared_pointer_wrapper_impl& other);
	atomic_shared_pointer_wrapper_impl& operator=(const shared_pointer_wrapper_impl<T,ReferenceCounter>& other);
	atomic_shared_pointer_wrapper_impl& operator=(atomic_shared_pointer_wrapper_impl&& other);
	atomic_shared_pointer_wrapper_impl& operator=(shared_pointer_wrapper_impl<T,ReferenceCounter>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl& operator=(const atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl& operator=(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl& operator=(atomic_shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other);
	template<typename TT>
	atomic_shared_pointer_wrapper_impl& operator=(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other);
	inline operator shared_pointer_wrapper_impl<T,ReferenceCounter>&();
	inline operator const shared_pointer_wrapper_impl<T,ReferenceCounter>&() const;
	inline bool operator==(std::nullptr_t) const;
	inline bool operator!=(std::nullptr_t) const;
	inline T* operator->();
	inline const T* operator->() const;
	inline T& operator*();
	inline const T& operator*() const;
	inline operator bool() const;
	void clear();
	inline T* get();
	inline const T* get() const;
	inline tagged_pointer_deleter get_deleter() const;
	inline bool empty() const;

private:
	mutable spin_lock m_barrier;
	shared_pointer_wrapper_impl<T,ReferenceCounter> m_ptr;
};

}

template<typename T>
using atomic_shared_pointer_wrapper = detail::atomic_shared_pointer_wrapper_impl<T,shared_reference_counter>;
template<typename T>
using atomic_distributed_pointer_wrapper = detail::atomic_shared_pointer_wrapper_impl<T,distributed_reference_counter>;

}

#include "ddk_atomic_shared_pointer_wrapper.inl"