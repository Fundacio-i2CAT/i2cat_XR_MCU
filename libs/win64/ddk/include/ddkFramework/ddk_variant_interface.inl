#pragma once

namespace ddk
{
namespace detail
{

template<typename Variant, size_t TypePos, typename ... Types>
_variant_interface<Variant,TypePos,Types...>::_variant_interface(const rawType& i_value)
{
    Variant::template construct<TypePos-1>(i_value);
}
template<typename Variant, size_t TypePos, typename ... Types>
_variant_interface<Variant,TypePos,Types...>::_variant_interface(rawType&& i_value)
{
    Variant::template construct<TypePos-1>(std::move(i_value));
}
template<typename Variant, size_t TypePos, typename ... Types>
_variant_interface<Variant,TypePos,Types...>& _variant_interface<Variant,TypePos,Types...>::operator=(const rawType& i_value)
{
    Variant::template assign<TypePos-1>(i_value);

    return *this;
}
template<typename Variant, size_t TypePos, typename ... Types>
_variant_interface<Variant,TypePos,Types...>& _variant_interface<Variant,TypePos,Types...>::operator=(rawType&& i_value)
{
    Variant::template assign<TypePos-1>(std::move(i_value));

    return *this;
}
template<typename Variant, size_t TypePos, typename ... Types>
bool _variant_interface<Variant,TypePos,Types...>::operator==(const rawType& other) const
{
    return Variant::template compare<TypePos-1>(other);
}
template<typename Variant, size_t TypePos, typename ... Types>
bool _variant_interface<Variant,TypePos,Types...>::operator==(rawType&& other) const
{
    return Variant::template compare<TypePos-1>(std::move(other));
}

}
}