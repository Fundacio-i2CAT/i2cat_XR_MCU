#pragma once

#include "ddk_arena.h"
#include "ddk_none.h"

namespace ddk
{

template<typename T>
class optional
{
	template<typename TT>
	friend class optional;

public:
    //publish relevant inner types
    typedef const typename embedded_type<T>::raw_type& reference_const_type;
    typedef typename embedded_type<T>::raw_type& reference_type;
    typedef typename embedded_type<T>::raw_type&& rreference_type;
    typedef typename embedded_type<T>::raw_type* pointer_type;
    typedef const typename embedded_type<T>::raw_type* pointer_const_type;

    inline optional();
    inline optional(detail::none_t);
    inline optional(const optional<T>& other);
    inline optional(optional<T>&& other);
    inline optional(reference_type val);
    inline optional(reference_const_type val);
    inline optional(rreference_type val);
    template<typename TT>
	inline optional(const optional<TT>& other);
    template<typename TT>
	inline optional(optional<TT>&& other);
    template<typename Arg>
    explicit optional(Arg&& i_arg, typename std::enable_if<std::is_convertible<Arg,optional<T>>::value == false>::type* = NULL);
    inline ~optional();
    inline optional<T>& operator=(const optional<T>& other);
    inline optional<T>& operator=(optional<T>&& other);
    template<typename TT>
	inline optional<T>& operator=(const optional<TT>& other);
    template<typename TT>
    inline optional<T>& operator=(optional<TT>&& other);
    inline optional<T>& operator=(detail::none_t);
    inline optional<T>& operator=(reference_type val);
    inline optional<T>& operator=(reference_const_type val);
    inline optional<T>& operator=(rreference_type val);
	inline bool empty() const;
    inline explicit operator bool() const;
    inline bool operator!() const;
    inline typename embedded_type<T>::cref_type get() const;
    inline typename embedded_type<T>::ref_type get();
    inline typename embedded_type<T>::cref_type get_value_or(typename embedded_type<T>::cref_type default_value) const;
    inline typename embedded_type<T>::ref_type get_value_or(typename embedded_type<T>::ref_type default_value);
    inline typename embedded_type<T>::cpointer_type get_ptr() const;
    inline typename embedded_type<T>::pointer_type get_ptr();
    inline T extract();
    inline typename embedded_type<T>::cref_type operator*() const;
    inline typename embedded_type<T>::ref_type operator*();
    inline typename embedded_type<T>::cpointer_type operator->() const;
    inline typename embedded_type<T>::pointer_type operator->();
    inline void swap(const optional<T>& other);

private:
	typed_arena<embedded_type<T>> m_storage;
};

}

#include "ddk_optional.inl"
