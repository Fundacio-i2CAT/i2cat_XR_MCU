
#include "ddk_iteration.h"
#include "ddk_iterable_impl.h"
#include "ddk_union_iterable_impl.h"
#include "ddk_intersection_iterable_impl.h"
#include "ddk_async.h"
#include "ddk_forwarding_iterable_value_callable.h"
#include "ddk_one_to_n_action_adapter.h"

namespace ddk
{

template<typename Iterable, typename IIterable>
Iterable make_iterable(IIterable&& i_iterable)
{
    typedef typename Iterable::traits traits;

    return Iterable{ make_distributed_reference<detail::iterable_impl<traits,IIterable>>(std::forward<IIterable>(i_iterable)) };
}

TEMPLATE(typename Iterable)
REQUIRED(IS_BASE_OF_ITERABLE(Iterable))
Iterable deduce_iterable(Iterable&& i_iterable)
{
	return i_iterable;
}

TEMPLATE(typename Container)
REQUIRED(IS_NOT_BASE_OF_ITERABLE(Container))
resolved_iterable<Container> deduce_iterable(Container&& i_iterable)
{
	typedef resolved_iterable<Container> iterable_type;
	typedef typename iterable_type::traits traits;

	return iterable_type{ make_distributed_reference<detail::iterable_impl<traits,Container>>(std::forward<Container>(i_iterable)) };
}

}

template<typename Function, typename Container>
ddk::detail::iterable<ddk::transformed_traits<ddk::resolved_iterable_traits_as<Container,typename ddk::mpl::aqcuire_callable_return_type<Function>::type>>> operator<<=(const ddk::detail::iterable_transform<Function>& i_lhs, Container&& i_rhs)
{
	typedef ddk::resolved_iterable_traits<Container> traits_t;
	typedef typename ddk::mpl::aqcuire_callable_return_type<Function>::type return_t;
	typedef ddk::resolved_iterable_traits_as<Container,return_t> return_traits_t;

	return ddk::detail::iterable<ddk::transformed_traits<return_traits_t>>(ddk::detail::make_iterable_impl<ddk::detail::transformed_iterable_impl<ddk::transformed_traits<return_traits_t>,traits_t,Function>>(share(ddk::deduce_iterable(i_rhs)),i_lhs.get_transform()));
}
template<typename Function,typename Container>
ddk::detail::iterable<ddk::resolved_iterable_traits<Container>> operator<<=(const ddk::detail::iterable_filter<Function>& i_lhs,Container&& i_rhs)
{
	typedef ddk::resolved_iterable_traits<Container> traits_t;

	return ddk::detail::iterable<traits_t>(ddk::detail::make_iterable_impl<ddk::detail::filtered_iterable_impl<traits_t,Function>>(share(ddk::deduce_iterable(i_rhs)),i_lhs.get_filter()));
}
template<typename T,typename Container>
ddk::detail::iterable<ddk::resolved_iterable_traits<Container>> operator<<=(const ddk::detail::iterable_order<T>& i_lhs,Container&& i_rhs)
{
	typedef ddk::resolved_iterable_traits<Container> traits_t;

	return ddk::detail::iterable<traits_t>(ddk::detail::make_iterable_impl<ddk::detail::ordered_iterable_impl<T,traits_t>>(share(ddk::deduce_iterable(i_rhs)),i_lhs.get_order()));
}
TEMPLATE(typename Function, typename Iterable)
REQUIRED(IS_CALLABLE(Function))
auto operator<<=(Function&& i_lhs,Iterable&& i_rhs)
{
	typedef typename std::remove_reference<Iterable>::type iterable_t;

	typedef typename ddk::mpl::static_if<ddk::concepts::is_iterable_valued_function<iterable_t,Function>,ddk::co_iteration<iterable_t,Function>,ddk::iteration<iterable_t,Function>>::type ret_type;

	return ret_type{ i_rhs,std::forward<Function>(i_lhs) };
}

namespace ddk
{

TEMPLATE(typename ... Iterables)
REQUIRED(IS_ITERABLE(Iterables)...)
detail::iterable<detail::union_iterable_traits<resolved_iterable_traits<Iterables>...>> concat(Iterables&& ... i_iterables)
{
    static_assert(mpl::num_types<Iterables...> != 0, "You shall provider more than 0 iterables");

    typedef detail::iterable<detail::union_iterable_traits<resolved_iterable_traits<Iterables>...>> ret_type;

	return ret_type{ ddk::detail::make_iterable_impl<ddk::detail::union_iterable_impl<ddk::detail::iterable<resolved_iterable_traits<Iterables>>...>>(deduce_iterable(std::forward<Iterables>(i_iterables)) ...) };
}

TEMPLATE(typename ... Iterables)
REQUIRED(IS_ITERABLE(Iterables)...)
detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> fusion(Iterables&& ... i_iterables)
{
    static_assert(mpl::num_types<Iterables...> != 0, "You shall provider more than 0 iterables");

    typedef detail::iterable<detail::intersection_iterable_traits<resolved_iterable_traits<Iterables>...>> ret_type;
	typedef typename ret_type::action action;

	return ret_type{ detail::make_iterable_impl<detail::intersection_iterable_impl<iter::one_to_n_diagonal_action_adapter<action>,detail::iterable<resolved_iterable_traits<Iterables>> ...>>(iter::one_to_n_diagonal_action_adapter<action>{},deduce_iterable(std::forward<Iterables>(i_iterables))...) };
}

}
