#pragma once

#include "ddk_reference_counter.h"
#include "ddk_reference_wrapper_deleter.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_tagged_pointer.h"
#include "ddk_reference_tag.h"
#include <type_traits>
#include <utility>

namespace ddk
{

template<typename>
class unique_pointer_wrapper;
template<typename>
class unique_reference_wrapper;

namespace detail
{

template<typename T>
inline unique_pointer_wrapper<T> __make_unique_pointer(T* i_data,const tagged_pointer<unique_reference_counter>& i_refCounter,const tagged_pointer_deleter& i_refDeleter);
template<typename T>
inline unique_pointer_wrapper<T> __make_unique_pointer(T* i_data,tagged_pointer<unique_reference_counter>&& i_refCounter,const tagged_pointer_deleter& i_refDeleter);

}

template<typename T>
class unique_pointer_wrapper
{
	friend inline T* get_raw_ptr(unique_pointer_wrapper i_ref)
	{
		return i_ref.m_data;
	}
	friend inline void set_raw_ptr(unique_pointer_wrapper& i_ref,T* i_value)
	{
		i_ref.m_data = i_value;
	}
	friend inline T* extract_raw_ptr(unique_pointer_wrapper& i_ref)
	{
		T* res = i_ref.m_data;

		i_ref.m_data = nullptr;

		return res;
	}
	friend inline void clear_ptr(unique_pointer_wrapper& i_ref)
	{
		i_ref.m_data = nullptr;
	}

	template<typename>
	friend class unique_pointer_wrapper;
	template<typename>
	friend class unique_reference_wrapper;
	template<typename TTT, typename TT>
	friend unique_reference_wrapper<TTT> static_unique_cast(unique_reference_wrapper<TT>);
	template<typename TTT, typename TT>
	friend unique_pointer_wrapper<TTT> static_unique_cast(unique_pointer_wrapper<TT>);
	template<typename TTT,typename TT>
	friend unique_pointer_wrapper<TTT> dynamic_unique_cast(unique_pointer_wrapper<TT>&&);
	template<typename TTT,typename TT>
	friend unique_pointer_wrapper<TTT> dynamic_unique_cast(unique_reference_wrapper<TT>&&);
	template<typename TT>
	friend unique_reference_wrapper<TT> const_unique_cast(unique_reference_wrapper<const TT>);
	template<typename TT>
	friend unique_pointer_wrapper<TT> const_unique_cast(unique_pointer_wrapper<const TT>);
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(unique_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_pointer_wrapper<TT> lend(const unique_pointer_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(unique_reference_wrapper<TT>&);
	template<typename TT>
	friend lent_reference_wrapper<TT> lend(const unique_reference_wrapper<TT>&);
	template<typename TT>
	friend unique_reference_wrapper<TT> promote_to_ref(unique_pointer_wrapper<TT>);
    template<typename TT>
	friend inline unique_pointer_wrapper<TT> detail::__make_unique_pointer(TT* i_data, const tagged_pointer<unique_reference_counter>& i_refCounter, const tagged_pointer_deleter& i_refDeleter);
    template<typename TT>
	friend inline unique_pointer_wrapper<TT> detail::__make_unique_pointer(TT* i_data, tagged_pointer<unique_reference_counter>&& i_refCounter, const tagged_pointer_deleter& i_refDeleter);

public:
	typedef tagged_pointer<unique_reference_counter> tagged_reference_counter;
	typedef T value_type;
	typedef typename std::add_const<T>::type const_value_type;
	typedef value_type& reference;
	typedef const_value_type& const_reference;
	typedef value_type&& rreference;
	typedef value_type* pointer;
	typedef const_value_type* const_pointer;
	typedef unique_pointer_wrapper<const_value_type> const_type;

	unique_pointer_wrapper(const unique_pointer_wrapper&) = delete;
	unique_pointer_wrapper& operator=(const unique_pointer_wrapper&) = delete;
	unique_pointer_wrapper();
	unique_pointer_wrapper(std::nullptr_t);
	unique_pointer_wrapper(unique_pointer_wrapper&& other);
	template<typename TT>
	unique_pointer_wrapper(unique_pointer_wrapper<TT>&& other);
	~unique_pointer_wrapper();
	unique_pointer_wrapper& operator=(std::nullptr_t);
	unique_pointer_wrapper& operator=(unique_pointer_wrapper&& other);
	template<typename TT>
	unique_pointer_wrapper& operator=(unique_pointer_wrapper<TT>&& other);
	inline bool operator==(std::nullptr_t) const;
	inline bool operator!=(std::nullptr_t) const;
	inline T* operator->();
	inline const T* operator->() const;
	inline T& operator*();
	inline const T& operator*() const;
	inline operator bool() const;
	inline void clear();
	inline T* get();
	inline const T* get() const;
	inline bool empty() const;
	inline const tagged_pointer_deleter& get_deleter() const;

private:
	unique_pointer_wrapper(T* i_data, const tagged_reference_counter& i_refCounter,const tagged_pointer_deleter& i_refDeleter);
	unique_pointer_wrapper(T* i_data, tagged_pointer<unique_reference_counter>&& i_refCounter,const tagged_pointer_deleter& i_refDeleter);
	void clearIfCounterVoid(bool i_hasRefs);
	inline unique_reference_counter* extract_reference_counter();

	T* m_data;
	tagged_reference_counter m_refCounter;
	tagged_pointer_deleter m_deleter;
};

}

#include "ddk_unique_pointer_wrapper.inl"