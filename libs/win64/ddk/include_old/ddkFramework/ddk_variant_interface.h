#pragma once

#include "ddk_variant_impl.h"
#include "ddk_template_helper.h"

namespace ddk
{
namespace detail
{

template<typename Variant, size_t TypePos, typename ... Types>
class _variant_interface;

template<typename Variant, size_t TypePos, typename ... Types>
class _variant_interface : public _variant_interface<Variant,TypePos-1,Types...>
{
    typedef typename mpl::nth_type_of<TypePos-1,Types...>::type CurrType;
    typedef typename std::remove_const<typename std::remove_reference<CurrType>::type>::type rawType;
    typedef _variant_interface<Variant,TypePos-1,Types...> variant_base_t;

public:
    using variant_base_t::variant_base_t;
    using variant_base_t::operator=;
    using variant_base_t::operator==;
    using variant_base_t::operator!=;
    using variant_base_t::construct;
    using variant_base_t::get;
    using variant_base_t::extract;
    using variant_base_t::empty;
    using variant_base_t::is;
	using variant_base_t::is_base_of;
	using variant_base_t::which;
    using variant_base_t::reset;
    using variant_base_t::swap;
    using variant_base_t::visit;

    _variant_interface() = default;
    _variant_interface(const rawType& i_value);
    _variant_interface(rawType&& i_value);
    _variant_interface& operator=(const rawType& i_value);
    _variant_interface& operator=(rawType&& i_value);
    bool operator==(const rawType& other) const;
    bool operator==(rawType&& other) const;
};

template<typename Variant, typename ... Types>
class _variant_interface<Variant,0,Types...> : public Variant
{
public:
    using Variant::operator=;
    using Variant::operator==;
    using Variant::operator!=;
    using Variant::construct;
    using Variant::get;
    using Variant::extract;
    using Variant::empty;
    using Variant::is;
	using Variant::is_base_of;
	using Variant::which;
    using Variant::reset;
    using Variant::swap;
    using Variant::visit;

    _variant_interface() = default;
};

template<typename Variant, typename ... Types>
class variant_interface : public detail::_variant_interface<Variant,mpl::get_num_types<Types...>(),Types...>
{
    typedef detail::_variant_interface<Variant,mpl::get_num_types<Types...>(),Types...> variant_base_t;

public:
    using variant_base_t::variant_base_t;
    using variant_base_t::operator=;
    using variant_base_t::operator==;
    using variant_base_t::operator!=;
    using variant_base_t::construct;
    using variant_base_t::get;
    using variant_base_t::extract;
    using variant_base_t::empty;
    using variant_base_t::is;
	using variant_base_t::is_base_of;
	using variant_base_t::which;
    using variant_base_t::reset;
    using variant_base_t::swap;
    using variant_base_t::visit;
};

}
}

#include "ddk_variant_interface.inl"
