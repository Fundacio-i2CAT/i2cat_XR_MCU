#pragma once

#include "ddk_iterable_traits.h"
#include "ddk_iterable_value.h"
#include <type_traits>

#define DEFINE_COMMON_TRAITS(TRAITS1,TRAITS2,COMMON_TRAITS) \
template<typename TT> \
COMMON_TRAITS<TT> friend traits_common_access(const TRAITS1<TT>&, const TRAITS2<TT>&); \
template<typename TT> \
COMMON_TRAITS<TT> friend traits_common_access(const TRAITS2<TT>&, const TRAITS1<TT>&);

namespace ddk
{
namespace detail
{

template<template<typename> typename Traits, typename T>
Traits<T> traits_common_access(const Traits<T>&, const Traits<T>&);

template<typename T>
struct const_forward_iterable_valued_traits : public forward_iterable_traits<T>
{
    typedef forward_iterable_traits<T> iterable_base_traits;
    typedef const_forwarded_value<T> iterable_value;
    typedef const_forwarded_value<const T> iterable_const_value;

	template<typename TT>
	using as = const_forward_iterable_valued_traits<TT>;
};

template<typename T>
struct forward_iterable_valued_traits : public forward_iterable_traits<T>
{
    DEFINE_COMMON_TRAITS(forward_iterable_valued_traits,const_forward_iterable_valued_traits,const_forward_iterable_valued_traits);

    typedef forward_iterable_traits<T> iterable_base_traits;
    typedef forwarded_value<T> iterable_value;
    typedef forwarded_value<const T> iterable_const_value;

	template<typename TT>
	using as = forward_iterable_valued_traits<TT>;
};

template<typename T>
struct const_bidirectional_iterable_valued_traits : public const_bidirectional_iterable_traits<T>
{
    DEFINE_COMMON_TRAITS(const_bidirectional_iterable_valued_traits,const_forward_iterable_valued_traits,const_forward_iterable_valued_traits);

    typedef const_bidirectional_iterable_traits<T> iterable_base_traits;
    typedef const_bidirectional_value<T> iterable_value;
    typedef const_bidirectional_value<const T> iterable_const_value;

	template<typename TT>
	using as = const_bidirectional_iterable_valued_traits<TT>;
};

template<typename T>
struct bidirectional_iterable_valued_traits : public bidirectional_iterable_traits<T>
{
    DEFINE_COMMON_TRAITS(bidirectional_iterable_valued_traits,const_bidirectional_iterable_valued_traits,const_bidirectional_iterable_valued_traits);
    DEFINE_COMMON_TRAITS(bidirectional_iterable_valued_traits,forward_iterable_valued_traits,forward_iterable_valued_traits);
    DEFINE_COMMON_TRAITS(bidirectional_iterable_valued_traits,const_forward_iterable_valued_traits,const_forward_iterable_valued_traits);

    typedef bidirectional_iterable_traits<T> iterable_base_traits;
    typedef bidirectional_value<T> iterable_value;
    typedef bidirectional_value<const T> iterable_const_value;

	template<typename TT>
	using as = bidirectional_iterable_valued_traits<TT>;
};

template<typename T>
struct const_random_access_iterable_valued_traits : public const_random_access_iterable_traits<T>
{
    DEFINE_COMMON_TRAITS(const_random_access_iterable_valued_traits,const_bidirectional_iterable_valued_traits,const_bidirectional_iterable_valued_traits);
    DEFINE_COMMON_TRAITS(const_random_access_iterable_valued_traits,const_forward_iterable_valued_traits,const_forward_iterable_valued_traits);

    typedef const_random_access_iterable_traits<T> iterable_base_traits;
    typedef const_random_accessed_value<T> iterable_value;
    typedef const_random_accessed_value<const T> iterable_const_value;

	template<typename TT>
	using as = const_random_access_iterable_valued_traits<TT>;
};

template<typename T>
struct random_access_iterable_valued_traits : public random_access_iterable_traits<T>
{
    DEFINE_COMMON_TRAITS(random_access_iterable_valued_traits,const_random_access_iterable_valued_traits,const_random_access_iterable_valued_traits);
    DEFINE_COMMON_TRAITS(random_access_iterable_valued_traits,bidirectional_iterable_valued_traits,bidirectional_iterable_valued_traits);
    DEFINE_COMMON_TRAITS(random_access_iterable_valued_traits,const_bidirectional_iterable_valued_traits,const_bidirectional_iterable_valued_traits);
    DEFINE_COMMON_TRAITS(random_access_iterable_valued_traits,forward_iterable_valued_traits,forward_iterable_valued_traits);
    DEFINE_COMMON_TRAITS(random_access_iterable_valued_traits,const_forward_iterable_valued_traits,const_forward_iterable_valued_traits);

    typedef random_access_iterable_traits<T> iterable_base_traits;
    typedef random_accessed_value<T> iterable_value;
    typedef random_accessed_value<const T> iterable_const_value;

	template<typename TT>
	using as = random_access_iterable_valued_traits<TT>;
};

template<typename ...>
struct union_iterable_traits_resolver;

template<typename Traits>
struct union_iterable_traits_resolver<Traits>
{
    typedef Traits traits;
    typedef typename traits::iterable_base_traits base_traits;
};

template<template <typename> typename Traits1, typename BaseType1, template<typename> typename Traits2, typename BaseType2, typename ... Traits>
struct union_iterable_traits_resolver<Traits1<BaseType1>,Traits2<BaseType2>,Traits...>
{
    typedef typename std::common_type<BaseType1,BaseType2>::type common_base_type;
    typedef typename mpl::static_if<std::is_const<BaseType1>::value || std::is_const<BaseType2>::value, const common_base_type,common_base_type>:: type cv_common_base_type;
    typedef decltype(traits_common_access(std::declval<Traits1<cv_common_base_type>>(),std::declval<Traits2<cv_common_base_type>>())) common_traits;
    typedef typename union_iterable_traits_resolver<common_traits,Traits...>::traits traits;
    typedef typename traits::iterable_base_traits base_traits;
};

template<typename ... Traits>
using union_iterable_traits = typename union_iterable_traits_resolver<Traits ...>::traits;
template<typename ... Traits>
using union_iterable_base_traits = typename union_iterable_traits_resolver<Traits ...>::base_traits;

template<typename ...>
struct intersection_iterable_traits_resolver;

template<typename Traits>
struct intersection_iterable_traits_resolver<Traits>
{
    typedef Traits traits;
    typedef typename traits::iterable_base_traits base_traits;
};

template<template <typename> typename Traits1, typename ... BaseType1, template<typename> typename Traits2, typename BaseType2, typename ... Traits>
struct intersection_iterable_traits_resolver<Traits1<values_tuple<BaseType1...>>,Traits2<BaseType2>,Traits...>
{
    typedef decltype(traits_common_access(std::declval<Traits1<values_tuple<BaseType1...,BaseType2>>>(),std::declval<Traits2<values_tuple<BaseType1...,BaseType2>>>())) common_traits;
    typedef typename intersection_iterable_traits_resolver<common_traits,Traits...>::traits traits;
    typedef typename traits::iterable_base_traits base_traits;
};

template<template <typename> typename Traits1, typename BaseType1, template<typename> typename Traits2, typename BaseType2, typename ... Traits>
struct intersection_iterable_traits_resolver<Traits1<BaseType1>,Traits2<BaseType2>,Traits...>
{
    typedef decltype(traits_common_access(std::declval<Traits1<values_tuple<BaseType1,BaseType2>>>(),std::declval<Traits2<values_tuple<BaseType1,BaseType2>>>())) common_traits;
    typedef typename intersection_iterable_traits_resolver<common_traits,Traits...>::traits traits;
    typedef typename traits::iterable_base_traits base_traits;
};

template<typename ... Traits>
using intersection_iterable_traits = typename intersection_iterable_traits_resolver<Traits...>::traits;
template<typename ... Traits>
using intersection_iterable_base_traits = typename intersection_iterable_traits_resolver<Traits...>::base_traits;

}
}
