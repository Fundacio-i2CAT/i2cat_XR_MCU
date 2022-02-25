#pragma once

#include "ddk_function.h"

namespace ddk
{

template<typename,typename = system_allocator>
class filter_function;

template<typename,typename>
struct get_resolved_filter_function;
template<typename ... Types,typename Allocator>
struct get_resolved_filter_function<mpl::type_pack<Types...>,Allocator>
{
    typedef filter_function<bool(Types...),Allocator> type;
};

template<typename Type, typename Allocator = system_allocator>
using resolved_filter_function = typename get_resolved_filter_function<Type,typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type>::type;

template<typename Callable, typename Allocator = system_allocator>
using resolved_filter_callable = resolved_filter_function<typename mpl::aqcuire_callable_args_type<Callable>::type,Allocator>;

template<typename ... Types, typename Allocator>
class filter_function<bool(Types...),Allocator> : public function<bool(Types...),Allocator>
{
    static_assert(mpl::get_num_types<Types...>() > 0, "You cannot create a filter function with zero arguments");

public:
    using function<bool(Types...)>::function;
    filter_function(const function<bool(Types...),Allocator>& other);
};

}

#include "ddk_filter_function.inl"

#include "ddk_filter_function_utils.h"
