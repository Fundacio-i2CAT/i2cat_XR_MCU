#pragma once

#include "ddk_reference_counter.h"
#include "ddk_reference_wrapper_deleter.h"
#include "ddk_tagged_pointer.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_concepts.h"
#include <utility>
#include <type_traits>

#define ENSURE_SCOPE_LIFETIME(_PTR) const auto ___##_PTR = _PTR;

namespace ddk
{

template<typename>
class weak_pointer_wrapper;

namespace detail
{

template<typename,typename>
class shared_reference_wrapper_impl;

template<typename T, typename ReferenceCounter>
class shared_pointer_wrapper_impl
{
	friend inline T* get_raw_ptr(shared_pointer_wrapper_impl i_ref)
	{
		return i_ref.m_data;
	}
	friend inline void set_raw_ptr(shared_pointer_wrapper_impl& i_ref,T* i_value)
	{
		i_ref.m_data = i_value;
	}
	friend inline T* extract_raw_ptr(shared_pointer_wrapper_impl& i_ref)
	{
		T* res = i_ref.m_data;

		i_ref.m_data = nullptr;

		return res;
	}
	friend inline void clear_ptr(shared_pointer_wrapper_impl& i_ref)
	{
		i_ref.m_data = nullptr;
	}

	template<typename,typename>
	friend class shared_pointer_wrapper_impl;
	template<typename,typename>
	friend class shared_reference_wrapper_impl;
	template<typename>
	friend class ddk::weak_pointer_wrapper;
	template<typename TT,typename TTT, typename RReferenceCounter>
	friend shared_pointer_wrapper_impl<TT,RReferenceCounter> __static_shared_cast(const shared_pointer_wrapper_impl<TTT,RReferenceCounter>&);
	template<typename TT,typename TTT, typename RReferenceCounter>
	friend shared_reference_wrapper_impl<TT,RReferenceCounter> __static_shared_cast(const shared_reference_wrapper_impl<TTT,RReferenceCounter>&);
	template<typename TT,typename TTT,typename RReferenceCounter>
	friend shared_pointer_wrapper_impl<TT,RReferenceCounter> __dynamic_shared_cast(const shared_pointer_wrapper_impl<TTT,RReferenceCounter>&);
	template<typename TT, typename TTT,typename RReferenceCounter>
	friend shared_pointer_wrapper_impl<TT,RReferenceCounter> __reinterpret_shared_cast(const shared_pointer_wrapper_impl<TTT,RReferenceCounter>&);
	template<typename TT, typename TTT,typename RReferenceCounter>
	friend shared_pointer_wrapper_impl<TT,RReferenceCounter> __reinterpret_shared_cast(const shared_reference_wrapper_impl<TTT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend shared_reference_wrapper_impl<TT,RReferenceCounter> __const_shared_cast(const shared_reference_wrapper_impl<const TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend shared_pointer_wrapper_impl<TT,RReferenceCounter> __const_shared_cast(const shared_pointer_wrapper_impl<const TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend lent_pointer_wrapper<TT> __lend(shared_pointer_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend lent_pointer_wrapper<TT> __lend(const shared_pointer_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend lent_reference_wrapper<TT> __lend(shared_reference_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend lent_reference_wrapper<TT> __lend(const shared_reference_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT,typename RReferenceCounter>
	friend shared_reference_wrapper_impl<TT,RReferenceCounter> __promote_to_ref(const shared_pointer_wrapper_impl<TT,RReferenceCounter>&);
	template<typename TT>
	friend ddk::weak_pointer_wrapper<TT> __weak(shared_pointer_wrapper_impl<TT,shared_reference_counter>&);
	template<typename TT>
	friend ddk::weak_pointer_wrapper<const TT> __weak(const shared_pointer_wrapper_impl<TT,shared_reference_counter>&);

public:
	typedef tagged_pointer<ReferenceCounter> tagged_reference_counter;
	typedef T value_type;
	typedef typename std::add_const<T>::type const_value_type;
	typedef value_type& reference;
	typedef const_value_type& const_reference;
	typedef value_type&& rreference;
	typedef value_type* pointer;
	typedef const_value_type* const_pointer;
	typedef shared_pointer_wrapper_impl<const_value_type,ReferenceCounter> const_type;

	shared_pointer_wrapper_impl();
	shared_pointer_wrapper_impl(std::nullptr_t);
	shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl& other);
	shared_pointer_wrapper_impl(shared_pointer_wrapper_impl&& other);
	template<typename TT>
	shared_pointer_wrapper_impl(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other);
	template<typename TT>
	shared_pointer_wrapper_impl(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other);
	~shared_pointer_wrapper_impl();
	shared_pointer_wrapper_impl& operator=(std::nullptr_t);
	shared_pointer_wrapper_impl& operator=(const shared_pointer_wrapper_impl& other);
	shared_pointer_wrapper_impl& operator=(shared_pointer_wrapper_impl&& other);
	template<typename TT>
	shared_pointer_wrapper_impl& operator=(const shared_pointer_wrapper_impl<TT,ReferenceCounter>& other);
	template<typename TT>
	shared_pointer_wrapper_impl& operator=(shared_pointer_wrapper_impl<TT,ReferenceCounter>&& other);
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
	shared_pointer_wrapper_impl(T* i_data, const tagged_reference_counter& i_refCounter,const tagged_pointer_deleter& i_refDeleter, bool i_alreadyIncremented = false);
	void clearIfCounterVoid(size_t i_currNumRefs);

	T* m_data;
	tagged_reference_counter m_refCounter;
	tagged_pointer_deleter m_deleter;
};

}

template<typename T>
using shared_pointer_wrapper = detail::shared_pointer_wrapper_impl<T,shared_reference_counter>;
template<typename T>
using distributed_pointer_wrapper = detail::shared_pointer_wrapper_impl<T,distributed_reference_counter>;

}

#include "ddk_shared_pointer_wrapper.inl"
