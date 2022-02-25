#pragma once

#include "ddk_atomic_shared_pointer_wrapper.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename T, typename ReferenceCounter>
class atomic_shared_reference_wrapper_impl : public atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>
{
public:
	using atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator*;
	using atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::operator->;
	using atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::get;
	using atomic_shared_pointer_wrapper_impl<T,ReferenceCounter>::clear;

	atomic_shared_reference_wrapper_impl(const atomic_shared_reference_wrapper_impl& other);
	atomic_shared_reference_wrapper_impl(const shared_reference_wrapper_impl<T,ReferenceCounter>& other);
	atomic_shared_reference_wrapper_impl(atomic_shared_reference_wrapper_impl&& other);
	atomic_shared_reference_wrapper_impl(shared_reference_wrapper_impl<T,ReferenceCounter>&& other);
	template<typename TT>
	atomic_shared_reference_wrapper_impl(const atomic_shared_reference_wrapper_impl<TT,ReferenceCounter>& other);
	template<typename TT>
	atomic_shared_reference_wrapper_impl(const shared_reference_wrapper_impl<TT,ReferenceCounter>& other);
	template<typename TT>
	atomic_shared_reference_wrapper_impl(atomic_shared_reference_wrapper_impl<TT,ReferenceCounter>&& other);
	template<typename TT>
	atomic_shared_reference_wrapper_impl(shared_reference_wrapper_impl<TT,ReferenceCounter>&& other);
	atomic_shared_reference_wrapper_impl& operator=(const atomic_shared_reference_wrapper_impl& other);
	atomic_shared_reference_wrapper_impl& operator=(const shared_reference_wrapper_impl<T,ReferenceCounter>& other);
	atomic_shared_reference_wrapper_impl& operator=(atomic_shared_reference_wrapper_impl&& other);
	atomic_shared_reference_wrapper_impl& operator=(shared_reference_wrapper_impl<T,ReferenceCounter>&& other);
	operator shared_reference_wrapper_impl<T,ReferenceCounter>&();
	operator const shared_reference_wrapper_impl<T,ReferenceCounter>&() const;
};

}

template<typename T>
using atomic_shared_reference_wrapper = detail::atomic_shared_reference_wrapper_impl<T,shared_reference_counter>;
template<typename T>
using atomic_distributed_reference_wrapper = detail::atomic_shared_reference_wrapper_impl<T,distributed_reference_counter>;

}

#include "ddk_atomic_shared_reference_wrapper.inl"