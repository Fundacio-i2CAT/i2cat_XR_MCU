#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_transformed_iterable_impl.h"
#include "ddk_filtered_iterable_impl.h"
#include "ddk_ordered_iterable_impl.h"
#include "ddk_iterable_action_resolver.h"
#include <array>

namespace ddk
{
namespace view
{

template<typename Type, typename Allocator>
inline detail::iterable_filter<function<bool(Type),Allocator>> filter(const function<bool(Type),Allocator>& i_functor);
TEMPLATE(typename Functor)
REQUIRES(IS_CALLABLE(Functor))
inline detail::iterable_filter<detail::resolved_callable<Functor>> filter(Functor&& i_functor);

template<typename T>
inline detail::iterable_order<T> order(T&& i_order);

TEMPLATE(typename Iterable, typename ... Predicates)
REQUIRES(IS_CALLABLE(Predicates)...)
inline std::array<decltype(deduce_iterable(std::declval<Iterable>())),mpl::num_types<Predicates...>+1> group_by(Iterable&& i_lhs, Predicates&& ... i_predicates);

}
}

#include "ddk_view_utils.inl"
