#pragma once

#include "ddk_shared_pointer_wrapper.h"
#include "ddk_dynamic_visitor.h"
#include "ddk_system_allocator.h"
#include "ddk_rtti_defs.h"
#include "ddk_any_value.h"
#include "ddk_concepts.h"
#include "ddk_smart_pointer_concepts.h"
#include "ddk_type_concepts.h"

namespace ddk
{

template<typename T, typename Allocator = system_allocator>
class inherited_value
{
	static_assert(std::is_polymorphic<T>::value, "You shall provide a polymorphic type since it relies on dynamic casts");

	template<typename TT, typename ... Args>
	friend inherited_value<TT> make_inherited_value(Args&& ... i_args);
	template<typename,typename>
	friend class inherited_value;
	friend inline lent_pointer_wrapper<T> lend(const inherited_value& i_value)
	{
		lend(i_value.m_value);
	}

public:
    typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T&& rreference;

	inherited_value() = default;
	inherited_value(const inherited_value& other) = default;
	inherited_value(inherited_value&& other) = default;
	template<typename TT>
	inherited_value(const distributed_pointer_wrapper<TT>& i_value);
	template<typename TT>
	inherited_value(const inherited_value<TT,Allocator>& other);
	template<typename TT>
	inherited_value(inherited_value<TT,Allocator>&& other);
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	inherited_value(Args&& ... i_args);
	inherited_value& operator=(const inherited_value& other);
	inherited_value& operator=(inherited_value&& other);
	template<typename TT>
	inherited_value& operator=(const inherited_value<TT,Allocator>& other);
	template<typename TT>
	inherited_value& operator=(inherited_value<TT,Allocator>&& other);
	inherited_value& operator=(std::nullptr_t);
	template<typename TT>
	inline bool is() const;
	inline operator bool() const;
	inline const rtti::TypeInfo& get_type_info() const;
	inline pointer operator->();
	inline const_pointer operator->() const;
	inline reference operator*();
	inline const_reference operator*() const;
	template<typename Interface, typename Visitor>
	inline bool may_visit() const;
	template<typename Interface, typename Visitor>
	void visit(Visitor&& i_visitor) const;

private:
	rtti::TypeInfo m_typeInfo;
	distributed_pointer_wrapper<T> m_value;
	Allocator m_allocator;
};

}

#include "ddk_inherited_value_concepts.h"
#include "ddk_inherited_value.inl"
#include "ddk_dynamic_multivisitor.h"
