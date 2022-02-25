
#include "ddk_function_utils.h"

namespace ddk
{
namespace detail
{

template<typename SuperClass,typename Return, typename ... Types>
Return intersection_function_executor<SuperClass,Return,mpl::type_pack<Types...>>::operator()(forwarded_arg<Types> ... i_args) const
{
    return execute(typename mpl::make_sequence<0,SuperClass::s_num_callables>::type{}, std::forward<forwarded_arg<Types>>(i_args)...);
}
template<typename SuperClass,typename Return, typename ... Types>
template<size_t ... Indexs>
Return intersection_function_executor<SuperClass,Return,mpl::type_pack<Types...>>::execute(const mpl::sequence<Indexs...>&, forwarded_arg<Types> ... i_args) const
{
	return Return{ ddk::eval(static_cast<const SuperClass*>(this)->m_callables.template get<Indexs>(),std::forward<forwarded_arg<Types>>(i_args) ...) ... };
}

template<typename Callable, typename ... Callables>
intersection_function<Callable,Callables...>::intersection_function(const Callable& i_arg, const Callables& ... i_callables)
: m_callables(i_arg,i_callables...)
{
}
template<typename Callable, typename ... Callables>
intersection_function<Callable,Callables...>::intersection_function(const tuple<Callable,Callables...>& i_callables)
: m_callables(i_callables)
{
}
template<typename Callable, typename ... Callables>
intersection_function<Callable,Callables...>::intersection_function(tuple<Callable,Callables...>&& i_callables)
: m_callables(std::move(i_callables))
{
}
template<typename Callable, typename ... Callables>
intersection_function<Callable,Callables...>::intersection_function(const intersection_function<Callable,Callables...>& other)
: m_callables(other.m_callables)
{
}
template<typename Callable, typename ... Callables>
intersection_function<Callable,Callables...>::intersection_function(intersection_function<Callable,Callables...>&& other)
: m_callables(std::move(other.m_callables))
{
}
template<typename Callable, typename ... Callables>
template<size_t Index>
const mpl::nth_type_of_t<Index,Callable,Callables...>& intersection_function<Callable,Callables...>::get_callable() const
{
    return m_callables.template get<Index>();
}

}
}
