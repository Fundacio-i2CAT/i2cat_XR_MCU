#pragma once

#include "ddk_shared_pointer_wrapper.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename T, typename ReferenceCounter>
class shared_reference_wrapper_impl : public shared_pointer_wrapper_impl<T,ReferenceCounter>
{
	template<typename,typename>
	friend class shared_reference_wrapper_impl;
	template<typename TTT, typename TT, typename RReferenceCounter>
	friend shared_reference_wrapper_impl<TTT,RReferenceCounter> __static_shared_cast(const shared_reference_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TTT, typename TT,typename RReferenceCounter>
	friend shared_reference_wrapper_impl<TTT,RReferenceCounter> __dynamic_shared_cast(const shared_reference_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend lent_reference_wrapper<TT> __lend(const shared_reference_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT, typename RReferenceCounter>
	friend inline shared_reference_wrapper_impl<TT,RReferenceCounter> __make_shared_reference(TT* i_data, const tagged_pointer<RReferenceCounter>& i_refCounter, const tagged_pointer_deleter& i_refDeleter);

	shared_reference_wrapper_impl(T* i_data, const tagged_pointer<ReferenceCounter>& i_refCounter, const tagged_pointer_deleter& i_refDeleter);

	using shared_pointer_wrapper_impl<T,ReferenceCounter>::operator bool;
	using shared_pointer_wrapper_impl<T,ReferenceCounter>::empty;

public:
	using typename shared_pointer_wrapper_impl<T,ReferenceCounter>::tagged_reference_counter;
	using typename shared_pointer_wrapper_impl<T,ReferenceCounter>::value_type;
	using typename shared_pointer_wrapper_impl<T,ReferenceCounter>::const_value_type;
	using typename shared_pointer_wrapper_impl<T,ReferenceCounter>::reference;
	using typename shared_pointer_wrapper_impl<T,ReferenceCounter>::const_reference;
	using typename shared_pointer_wrapper_impl<T,ReferenceCounter>::rreference;
	using typename shared_pointer_wrapper_impl<T,ReferenceCounter>::pointer;
	using typename shared_pointer_wrapper_impl<T,ReferenceCounter>::const_pointer;
	typedef shared_pointer_wrapper_impl<const_value_type,ReferenceCounter> const_type;
	using shared_pointer_wrapper_impl<T,ReferenceCounter>::operator*;
    using shared_pointer_wrapper_impl<T,ReferenceCounter>::operator->;
    using shared_pointer_wrapper_impl<T,ReferenceCounter>::get;
    using shared_pointer_wrapper_impl<T,ReferenceCounter>::clear;

	shared_reference_wrapper_impl(const shared_reference_wrapper_impl& other);
	shared_reference_wrapper_impl(shared_reference_wrapper_impl&& other);
	template<typename TT>
	shared_reference_wrapper_impl(const shared_reference_wrapper_impl<TT,ReferenceCounter>& other);
	template<typename TT>
	shared_reference_wrapper_impl(shared_reference_wrapper_impl<TT,ReferenceCounter>&& other);
    shared_reference_wrapper_impl& operator=(const shared_reference_wrapper_impl& other);
    shared_reference_wrapper_impl& operator=(shared_reference_wrapper_impl&& other);
};

}

template<typename T>
using shared_reference_wrapper = detail::shared_reference_wrapper_impl<T,shared_reference_counter>;
template<typename T>
using distributed_reference_wrapper = detail::shared_reference_wrapper_impl<T,distributed_reference_counter>;

}

#include "ddk_shared_reference_wrapper.inl"
