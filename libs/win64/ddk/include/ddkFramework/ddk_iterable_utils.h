#pragma once

#include "ddk_union_iterable.h"
#include "ddk_intersection_iterable.h"
#include "ddk_transformed_iterable_impl.h"
#include "ddk_filtered_iterable_impl.h"
#include "ddk_ordered_iterable_impl.h"
#include "ddk_concepts.h"
#include "ddk_iterable_concepts.h"
#include "ddk_container_concepts.h"

namespace ddk
{

template<typename Iterable>
using resolved_iterable = detail::iterable_correspondence<typename std::remove_reference<Iterable>::type>;
template<typename Iterable>
using resolved_iterable_traits = typename detail::iterable_correspondence<typename std::remove_reference<Iterable>::type>::traits;
template<typename Iterable, typename T>
using resolved_iterable_traits_as = typename detail::iterable_correspondence<typename std::remove_reference<Iterable>::type>::traits::template as<T>;
template<typename Iterable>
using resolved_iterable_action = typename detail::iterable_action_correspondence<typename std::remove_reference<Iterable>::type>;

template<typename Iterable, typename IIterable>
inline Iterable make_iterable(IIterable&& i_iterable);

TEMPLATE(typename Iterable)
REQUIRES(IS_BASE_OF_ITERABLE(Iterable))
inline Iterable deduce_iterable(Iterable&& i_iterable);

TEMPLATE(typename Container)
REQUIRES(IS_NOT_BASE_OF_ITERABLE(Container))
inline resolved_iterable<Container> deduce_iterable(Container&& i_iterable);

template<typename T>
using deduced_iterable = decltype(deduce_iterable(std::declval<const T&>()));

template<typename>
struct transformed_traits_resolver;
template<template<typename> typename Traits>
struct transformed_traits_resolver<Traits<void>>
{
};
template<template<typename> typename Traits, typename Return>
struct transformed_traits_resolver<Traits<Return>>
{
    typedef typename mpl::static_if<std::is_reference<Return>::value,Traits<typename std::remove_reference<Return>::type>,Traits<typename std::add_const<Return>::type>>::type type;
};

template<typename Traits>
using transformed_traits = typename transformed_traits_resolver<Traits>::type;

}

template<typename Function, typename Container>
inline ddk::detail::iterable<ddk::transformed_traits<ddk::resolved_iterable_traits_as<Container,typename ddk::mpl::aqcuire_callable_return_type<Function>::type>>> operator<<=(const ddk::detail::iterable_transform<Function>& i_lhs, Container&& i_rhs);

template<typename Function,typename Container>
inline ddk::detail::iterable<ddk::resolved_iterable_traits<Container>> operator<<=(const ddk::detail::iterable_filter<Function>& i_lhs,Container&& i_rhs);

template<typename T,typename Container>
inline ddk::detail::iterable<ddk::resolved_iterable_traits<Container>> operator<<=(const ddk::detail::iterable_order<T>& i_lhs,Container&& i_rhs);

TEMPLATE(typename Function,typename Iterable)
REQUIRES(IS_CALLABLE(Function))
inline auto operator<<=(Function&& i_lhs, Iterable&&i_rhs);

namespace ddk
{

TEMPLATE(typename ... Iterables)
REQUIRES(IS_ITERABLE(Iterables)...)
inline detail::iterable<detail::union_iterable_traits<resolved_iterable_traits<Iterables>...>> concat(Iterables&& ... i_iterables);

TEMPLATE(typename ... Iterables)
REQUIRES(IS_ITERABLE(Iterables)...)
inline detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> fusion(Iterables&& ... i_iterables);

}

#include "ddk_iterable_utils.inl"
#include "ddk_iterable_algorithm.h"