
#include "ddk_function_utils.h"

namespace ddk
{
namespace view
{

template<typename Type, typename Allocator>
detail::iterable_filter<function<bool(Type),Allocator>> filter(const function<bool(Type),Allocator>& i_functor)
{
	return detail::iterable_filter<function<bool(Type),Allocator>>(i_functor);
}
TEMPLATE(typename Functor)
REQUIRED(IS_CALLABLE(Functor))
detail::iterable_filter<detail::resolved_callable<Functor>> filter(Functor&& i_functor)
{
	return detail::iterable_filter<detail::resolved_callable<Functor>>(make_function(std::forward<Functor>(i_functor)));
}
template<typename T>
detail::iterable_order<T> order(T&& i_order)
{
	return detail::iterable_order<T>(i_order);
}
TEMPLATE(typename Iterable,typename ... Predicates)
REQUIRED(IS_CALLABLE(Predicates)...)
std::array<decltype(deduce_iterable(std::declval<Iterable>())),mpl::num_types<Predicates...>+1> group_by(Iterable&& i_lhs, Predicates&& ... i_predicates)
{
	return { filter(deduce_function(i_predicates)) <<= deduce_iterable(i_lhs) ..., filter((!deduce_function(i_predicates) && ...)) <<= deduce_iterable(i_lhs)};
}

}
}
