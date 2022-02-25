#pragma once

#include "ddk_iterable_interface.h"
#include "ddk_iterator_concepts.h"
#include "ddk_container_concepts.h"

#define IS_BASE_OF_ITERABLE(_TYPE) \
	typename std::enable_if<std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value>::type

#define IS_BASE_OF_ITERABLE_COND(_TYPE) \
	std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value

#define IS_NOT_BASE_OF_ITERABLE(_TYPE) \
	typename std::enable_if<std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value == false>::type

#define IS_NOT_BASE_OF_ITERABLE_COND(_TYPE) \
	(std::is_base_of<ddk::detail::iterable_interface,typename std::remove_reference<_TYPE>::type>::value == false)

#define IS_FORWARD_ITERABLE(_TYPE) \
    HAS_ITERATOR_DEFINED(_TYPE),IS_FORWARD_ITERATOR(typename _TYPE::iterator)

#define IS_BIDIRECTIONAL_ITERABLE(_TYPE) \
    IS_FORWARD_ITERABLE(_TYPE),IS_BIDIRECTIONAL_ITERATOR(typename _TYPE::iterator)

#define IS_RANDOM_ACCESS_ITERABLE(_TYPE) \
    IS_BIDIRECTIONAL_ITERABLE(_TYPE),IS_RANDOM_ACCESS_ITERATOR(typename _TYPE::iterator)

#define IS_EXCLUSIVE_FORWARD_ITERABLE(_TYPE) \
    HAS_ITERATOR_DEFINED(_TYPE),IS_EXCLUSIVE_FORWARD_ITERATOR(typename _TYPE::iterator)

#define IS_EXCLUSIVE_BIDIRECTIONAL_ITERABLE(_TYPE) \
    HAS_ITERATOR_DEFINED(_TYPE), IS_EXCLUSIVE_BIDIRECTIONAL_ITERATOR(typename _TYPE::iterator)

#define IS_EXCLUSIVE_RANDOM_ACCESS_ITERABLE(_TYPE) \
    HAS_ITERATOR_DEFINED(_TYPE),IS_EXCLUSIVE_RANDOM_ACCESS_ITERATOR(typename _TYPE::iterator)

#define IS_ITERABLE(_TYPE) \
    typename std::enable_if<IS_CONTAINER_COND(_TYPE) || IS_BASE_OF_ITERABLE_COND(_TYPE)>::type

namespace ddk
{
namespace concepts
{

template<typename IterableValue,typename T,size_t ... Indexs>
typename mpl::static_if<mpl::holds_type_for_some_type<std::is_constructible,IterableValue&&,typename T::template nth_type<Indexs>...>(),std::true_type,std::false_type>::type _resolve_iterable_valued_function(const mpl::sequence<Indexs...>&);
template<typename Iterable,typename Function,typename T = typename mpl::aqcuire_callable_args_type<Function>::type>
decltype(_resolve_iterable_valued_function<typename Iterable::iterable_value,T>(typename mpl::make_sequence<0,T::size()>::type{})) resolve_iterable_valued_function(const Iterable&,Function&);
template<typename Iterable,typename Function,typename = decltype(std::declval<Function>()(std::declval<typename Iterable::iterable_value>()))>
std::true_type resolve_iterable_valued_function(const Iterable&,const Function&);
template<typename ... T>
std::false_type resolve_iterable_valued_function(const T& ...);

template<typename Iterable,typename Function>
inline constexpr bool is_iterable_valued_function = decltype(resolve_iterable_valued_function(std::declval<Iterable>(),std::declval<mpl::remove_qualifiers<Function>&>()))::value;

}
}
