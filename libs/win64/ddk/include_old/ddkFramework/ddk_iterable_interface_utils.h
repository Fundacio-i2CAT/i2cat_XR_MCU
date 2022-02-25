#pragma once

#include "ddk_concepts.h"
#include "ddk_tuple_values.h"

namespace ddk
{
namespace detail
{

TEMPLATE(typename Function,typename Arg)
REQUIRES_COND(is_values_tuple<Arg>::value == false)
inline auto call_iterable_payload(const mpl::sequence<0>&,Function&& i_function,Arg&& i_arg);

template<size_t ... Indexs,typename Function,typename ... Args>
inline auto call_iterable_payload(const mpl::sequence<Indexs...>&,Function&& i_function,values_tuple<Args...> i_arg);

}
}

#include "ddk_iterable_interface_utils.inl"