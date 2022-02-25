#pragma once

#include "ddk_variant_concepts.h"
#include "ddk_callable.h"

namespace ddk
{

template<typename Type>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type),IS_COPY_CONSTRUCTIBLE(TTypes)...)
constexpr variant<Type>::variant(const variant<TTypes ...>& other)
: m_value(other.template get<Type>())
{
}
template<typename Type>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type),IS_MOVE_CONSTRUCTIBLE(TTypes)...)
constexpr variant<Type>::variant(variant<TTypes...>&& other)
: m_value(std::move(other).template extract<Type>())
{
}
template<typename Type>
TEMPLATE(typename T)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(Type,T))
constexpr variant<Type>::variant(T&& i_value)
: m_value(std::forward<T>(i_value))
{
}
template<typename Type>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type),IS_COPY_ASSIGNABLE(TTypes)...)
variant<Type>& variant<Type>::operator=(const variant<TTypes...>& other)
{
	m_value = other.template get<Type>();

	return *this;
}
template<typename Type>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Type),IS_MOVE_ASSIGNABLE(TTypes)...)
variant<Type>& variant<Type>::operator=(variant<TTypes...>&& other)
{
	m_value = std::move(other).template extract<Type>();

	return *this;
}
template<typename Type>
TEMPLATE(typename T)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(Type,T))
variant<Type>& variant<Type>::operator=(T&& i_value)
{
	m_value = std::forward<T>(i_value);

	return *this;
}
template<typename Type>
template<typename T>
constexpr bool variant<Type>::operator==(T&& other) const
{
	return m_value == std::forward<T>(other);
}
template<typename Type>
template<typename T>
constexpr bool variant<Type>::operator!=(T&& other) const
{
	return m_value != std::forward<T>(other);
}
template<typename Type>
template<typename TType>
constexpr typename embedded_type<TType>::cref_type variant<Type>::get() const
{
	return m_value;
}
template<typename Type>
template<typename TType>
constexpr typename embedded_type<TType>::ref_type variant<Type>::get()
{
	return m_value;
}
template<typename Type>
template<typename TType>
TType variant<Type>::extract() &&
{
	return std::move(m_value);
}
template<typename Type>
template<typename TType>
constexpr bool variant<Type>::is() const
{
	return std::is_same<Type,TType>::value;
}
template<typename Type>
template<typename TType>
constexpr bool variant<Type>::contains()
{
	return std::is_same<TType,Type>::value;
}
template<typename Type>
template<typename TType>
constexpr typename embedded_type<TType>::cref_type variant<Type>::get_as() const
{
	return m_value;
}
template<typename Type>
template<typename TType>
constexpr typename embedded_type<TType>::ref_type variant<Type>::get_as()
{
	return m_value;
}
template<typename Type>
template<typename TType>
constexpr bool variant<Type>::is_base_of() const
{
	return std::is_base_of<Type,TType>::value;
}
template<typename Type>
template<size_t Pos>
constexpr typename embedded_type<Type>::cref_type variant<Type>::get() const
{
	static_assert(Pos == 0, "Index out of bounds");

	return m_value;
}
template<typename Type>
template<size_t Pos>
constexpr typename embedded_type<Type>::ref_type variant<Type>::get()
{
	static_assert(Pos == 0,"Index out of bounds");

	return m_value;
}
template<typename Type>
template<size_t Pos>
Type variant<Type>::extract() &&
{
	static_assert(Pos == 0,"Index out of bounds");

	return std::move(m_value);
}
template<typename Type>
template<size_t Pos>
constexpr bool variant<Type>::is() const
{
	return Pos == 0;
}
template<typename Type>
constexpr char variant<Type>::which() const
{
	return 0;
}
template<typename Type>
TEMPLATE(typename Visitor)
REQUIRED(IS_CALLABLE(Visitor,Type))
constexpr auto variant<Type>::visit(Visitor&& visitor)
{
	typedef decltype(std::declval<Visitor>()(std::declval<Type>())) return_type;

	if constexpr (std::is_same<void,return_type>::value)
	{
		visitor(m_value);
	}
	else
	{
		return visitor(m_value);
	}
}
template<typename Type>
TEMPLATE(typename Visitor)
REQUIRED(IS_CALLABLE(Visitor,Type))
constexpr auto variant<Type>::visit(Visitor&& visitor) const
{
	typedef decltype(std::declval<Visitor>()(std::declval<Type>())) return_type;

	if constexpr(std::is_same<void,return_type>::value)
	{
		visitor(m_value);
	}
	else
	{
		return visitor(m_value);
	}
}
template<typename Type>
template<typename Visitor, typename ... Args>
constexpr auto variant<Type>::visit(Args&& ... i_args) const
{
	typedef decltype(std::declval<Visitor>()(std::declval<Type>())) return_type;
	
	const Visitor _visitor(std::forward<Args>(i_args)...);

	if constexpr(std::is_same<void,return_type>::value)
	{
		_visitor(m_value);
	}
	else
	{
		return _visitor(m_value);
	}
}

template<typename ... Types>
TEMPLATE(typename TType)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...),IS_COPY_CONSTRUCTIBLE(TType))
constexpr variant<Types...>::variant(const variant<TType>&other)
: detail::variant_impl<Types...>(mpl::static_number<mpl::type_match_pos<TType,Types...>>{},other.m_value)
{
}
template<typename ... Types>
TEMPLATE(typename TType)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TType>,Types...),IS_MOVE_CONSTRUCTIBLE(TType))
constexpr variant<Types...>::variant(variant<TType> && other)
: detail::variant_impl<Types...>(mpl::static_number<mpl::type_match_pos<TType,Types...>>{},std::move(other.m_value))
{
}
template<typename ... Types>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...),IS_COPY_CONSTRUCTIBLE(TTypes)...)
constexpr variant<Types...>::variant(const variant<TTypes...>& other)
: detail::variant_impl<Types...>(other)
{
}
template<typename ... Types>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...),IS_MOVE_CONSTRUCTIBLE(TTypes)...)
constexpr variant<Types...>::variant(variant<TTypes...>&& other)
: detail::variant_impl<Types...>(std::move(other))
{
}
template<typename ... Types>
TEMPLATE(typename T)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
constexpr variant<Types...>::variant(T&& i_value)
: detail::variant_impl<Types...>(mpl::static_number<mpl::type_match_pos<T,Types...>>{},std::forward<T>(i_value))
{
	static_assert(mpl::is_among_constructible_types<T,Types...>,"You shall provide convertible type");
}
template<typename ... Types>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...),IS_COPY_ASSIGNABLE(TTypes)...)
variant<Types...>& variant<Types...>::operator=(const variant<TTypes...>& other)
{
	detail::variant_impl<Types...>::operator=(other);

	return *this;
}
template<typename ... Types>
TEMPLATE(typename ... TTypes)
REQUIRED(IS_NOT_AMONG_CONSTRUCTIBLE_TYPES(variant<TTypes...>,Types...),IS_MOVE_ASSIGNABLE(TTypes)...)
variant<Types...>& variant<Types...>::operator=(variant<TTypes...>&& other)
{
	detail::variant_impl<Types...>::operator=(std::move(other));

	return *this;
}
template<typename ... Types>
TEMPLATE(typename T)
REQUIRED(IS_AMONG_CONSTRUCTIBLE_TYPES(T,Types...))
variant<Types...>& variant<Types...>::operator=(T&& i_value)
{
	static_assert(mpl::is_among_constructible_types<T,Types...>,"You shall provide convertible type");

	detail::variant_impl<Types...>::template assign<mpl::type_match_pos<T,Types...>>(std::forward<T>(i_value));

	return *this;
}
template<typename ... Types>
template<typename T>
constexpr bool variant<Types...>::operator==(T&& other) const
{
	if constexpr(concepts::is_variant_v<T>)
	{
		return detail::variant_impl<Types...>::operator==(std::forward<T>(other));
	}
	else
	{
		static_assert(mpl::is_among_constructible_types<T,Types...>,"You shall provide convertible type");

		return detail::variant_impl<Types...>::template compare<mpl::type_match_pos<T,Types...>>(std::forward<T>(other));
	}
}
template<typename ... Types>
template<typename T>
constexpr bool variant<Types...>::operator!=(T&& other) const
{
	if constexpr(concepts::is_variant_v<T>)
	{
		return detail::variant_impl<Types...>::operator!=(std::forward<T>(other));
	}
	else
	{
		static_assert(mpl::is_among_constructible_types<T,Types...>,"You shall provide convertible type");

		return detail::variant_impl<Types...>::template compare<mpl::type_match_pos<T,Types...>>(std::forward<T>(other)) == false;
	}
}
template<typename ... Types>
template<typename Type>
constexpr bool variant<Types...>::contains()
{
	return mpl::is_among_types<Type,Types...>;
}

TEMPLATE(typename Return, typename Visitor,typename Variant)
REQUIRED(IS_VARIANT(Variant))
constexpr auto visit(Visitor&& i_visitor,Variant&& i_variant)
{
	if constexpr(std::is_same<void,Return>::value)
	{
		i_variant.visit(i_visitor);
	}
	else
	{
		return i_variant.visit(i_visitor);
	}
}
TEMPLATE(typename Visitor,typename Variant)
REQUIRED(IS_VARIANT(Variant))
constexpr auto visit(Visitor&& i_visitor,Variant&& i_variant)
{
	typedef decltype(i_variant.visit(std::declval<Visitor>())) return_type;

	if constexpr(std::is_same<void,return_type>::value)
	{
		i_variant.visit(i_visitor);
	}
	else
	{
		return i_variant.visit(i_visitor);
	}
}

TEMPLATE(typename Return, typename Visitor,typename Variant)
REQUIRED(IS_VARIANT(Variant))
constexpr Return visit(Variant&& i_variant)
{
	const Visitor _visitor;

	if constexpr (std::is_same<void,Return>::value)
	{
		i_variant.visit(_visitor);
	}
	else
	{
		return i_variant.visit(_visitor);
	}
}
TEMPLATE(typename Visitor,typename Variant)
REQUIRED(IS_VARIANT(Variant))
constexpr auto visit(Variant&& i_variant)
{
	typedef decltype(i_variant.visit(std::declval<Visitor>())) return_type;

	const Visitor _visitor;

	if constexpr(std::is_same<void,return_type>::value)
	{
		i_variant.visit(_visitor);
	}
	else
	{
		return i_variant.visit(_visitor);
	}
}

}
