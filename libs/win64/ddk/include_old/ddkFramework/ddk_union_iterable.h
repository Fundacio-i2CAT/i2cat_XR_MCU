#pragma once

#include "ddk_union_iterable_impl.h"

namespace ddk
{
namespace detail
{

template<typename ...>
class intersection_iterable;

template<typename ... Iterables>
class union_iterable
{
    friend inline iterable<union_iterable_traits<typename Iterables::traits...>> as_iterable(const union_iterable<Iterables...>& i_iterable)
    {
        return iterable<union_iterable_traits<typename Iterables::traits...>>(i_iterable.m_iterableImpl);
    }

public:
    typedef iterable<union_iterable_traits<typename Iterables::traits...>> related_iterable;
    typedef typename related_iterable::traits related_traits;

    union_iterable(shared_reference_wrapper<union_iterable_impl<Iterables...>> i_iterableImpl);

    related_iterable as_iterable() const;
    template<typename Iterable>
    operator Iterable() const;

private:
    shared_reference_wrapper<union_iterable_impl<Iterables...>> m_iterableImpl;
};

template<typename>
struct union_iterable_traits_adaptor;

template<template<typename>typename Traits, typename IterableValue>
struct union_iterable_traits_adaptor<iterable<Traits<IterableValue>>>
{
template<typename TransformedValue>
using transformed_traits = Traits<TransformedValue>;
};

template<typename TransformedValue, typename ... Iterables>
using union_iterable_transformed_traits = typename union_iterable_traits_adaptor<typename union_iterable<Iterables...>::related_iterable>::template transformed_traits<TransformedValue>;

}
}

#include "ddk_union_iterable.inl"
