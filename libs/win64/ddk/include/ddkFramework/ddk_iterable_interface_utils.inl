
#include "ddk_function_utils.h"

namespace ddk
{
namespace detail
{

TEMPLATE(typename Function,typename Arg)
REQUIRED_COND(is_values_tuple<Arg>::value == false)
auto call_iterable_payload(const mpl::sequence<0>&,Function&& i_function,Arg&& i_arg)
{
    typedef typename mpl::aqcuire_callable_return_type<mpl::remove_qualifiers<Function>>::type return_t;

    if constexpr(std::is_same<return_t,void>::value)
    {
        eval(std::forward<Function>(i_function),std::forward<Arg>(i_arg));
    }
    else
    {
        return eval(std::forward<Function>(i_function),std::forward<Arg>(i_arg));
    }
}

template<size_t ... Indexs,typename Function,typename ... Args>
auto call_iterable_payload(const mpl::sequence<Indexs...>&,Function&& i_function,values_tuple<Args...> i_arg)
{
    typedef typename mpl::aqcuire_callable_return_type<mpl::remove_qualifiers<Function>>::type return_t;

    if constexpr(std::is_same<return_t,void>::value)
    {
        eval(std::forward<Function>(i_function),std::forward<decltype(i_arg.template get<Indexs>())>(i_arg.template get<Indexs>())...);
    }
    else
    {
        return eval(std::forward<Function>(i_function),std::forward<decltype(i_arg.template get<Indexs>())>(i_arg.template get<Indexs>())...);
    }
}

}
}
