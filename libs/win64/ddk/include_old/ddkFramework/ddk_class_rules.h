#pragma once

namespace ddk
{
namespace detail
{

template<bool,bool,typename...>
struct assignable_rule_impl;

template<typename ... T>
struct assignable_rule_impl<true,true,T...>
{
	assignable_rule_impl() = default;
	assignable_rule_impl(const assignable_rule_impl&) = default;
	assignable_rule_impl(assignable_rule_impl&&) = default;

	assignable_rule_impl& operator=(const assignable_rule_impl&) = default;
	assignable_rule_impl& operator=(assignable_rule_impl&&) = default;
};
template<typename ... T>
struct assignable_rule_impl<false,true,T...>
{
	assignable_rule_impl() = default;
	assignable_rule_impl(const assignable_rule_impl&) = default;
	assignable_rule_impl(assignable_rule_impl&&) = default;

	assignable_rule_impl& operator=(const assignable_rule_impl&) = delete;
	assignable_rule_impl& operator=(assignable_rule_impl&&) = default;
};
template<typename ... T>
struct assignable_rule_impl<true,false,T...>
{
	assignable_rule_impl() = default;
	assignable_rule_impl(const assignable_rule_impl&) = default;
	assignable_rule_impl(assignable_rule_impl&&) = default;

	assignable_rule_impl& operator=(const assignable_rule_impl&) = default;
	assignable_rule_impl& operator=(assignable_rule_impl&&) = delete;
};
template<typename ... T>
struct assignable_rule_impl<false,false,T...>
{
	assignable_rule_impl() = default;
	assignable_rule_impl(const assignable_rule_impl&) = default;
	assignable_rule_impl(assignable_rule_impl&&) = default;

	assignable_rule_impl& operator=(const assignable_rule_impl&) = delete;
	assignable_rule_impl& operator=(assignable_rule_impl&&) = delete;
};

template<bool,bool,typename...>
struct constructible_rule_impl;

template<typename ... T>
struct constructible_rule_impl<true,true,T...>
{
	constructible_rule_impl() = default;
	constructible_rule_impl(const constructible_rule_impl&) = default;
	constructible_rule_impl(constructible_rule_impl&&) = default;

	constructible_rule_impl& operator=(const constructible_rule_impl&) = default;
	constructible_rule_impl& operator=(constructible_rule_impl&&) = default;
};
template<typename ... T>
struct constructible_rule_impl<false,true,T...>
{
	constructible_rule_impl() = default;
	constructible_rule_impl(const constructible_rule_impl&) = delete;
	constructible_rule_impl(constructible_rule_impl&&) = default;

	constructible_rule_impl& operator=(const constructible_rule_impl&) = default;
	constructible_rule_impl& operator=(constructible_rule_impl&&) = default;
};
template<typename ... T>
struct constructible_rule_impl<true,false,T...>
{
	constructible_rule_impl() = default;
	constructible_rule_impl(const constructible_rule_impl&) = default;
	constructible_rule_impl(constructible_rule_impl&&) = delete;

	constructible_rule_impl& operator=(const constructible_rule_impl&) = default;
	constructible_rule_impl& operator=(constructible_rule_impl&&) = default;
};
template<typename ... T>
struct constructible_rule_impl<false,false,T...>
{
	constructible_rule_impl() = default;
	constructible_rule_impl(const constructible_rule_impl&) = delete;
	constructible_rule_impl(constructible_rule_impl&&) = delete;

	constructible_rule_impl& operator=(const constructible_rule_impl&) = default;
	constructible_rule_impl& operator=(constructible_rule_impl&&) = default;
};

}

template<typename ... T>
using constructible_rule = detail::constructible_rule_impl<(std::is_copy_constructible<T>::value && ...),(std::is_move_constructible<T>::value && ...),T...>;
template<typename ... T>
using assignable_rule = detail::assignable_rule_impl<(std::is_copy_assignable<T>::value && ...),(std::is_move_assignable<T>::value && ...),T...>;

template<typename ... T>
class contravariant_rules : constructible_rule<T...>, assignable_rule<T...>
{
};

}