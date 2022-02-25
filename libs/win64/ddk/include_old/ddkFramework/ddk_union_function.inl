
#include "ddk_function_utils.h"

namespace ddk
{
namespace detail
{

template<typename SuperClass,typename Return, typename ... Types>
Return union_function_executor<SuperClass,Return,mpl::type_pack<Types...>>::operator()(forwarded_arg<Types> ... i_args) const
{
    return execute(typename mpl::make_sequence<0,SuperClass::s_num_callables>::type{}, ddk::make_tuple(std::forward<forwarded_arg<Types>>(i_args)...));
}
template<typename SuperClass,typename Return, typename ... Types>
template<size_t ... Indexs, typename ... Args>
Return union_function_executor<SuperClass,Return,mpl::type_pack<Types...>>::execute(const mpl::sequence<Indexs...>&, const tuple<Args...>& i_args) const
{
    static const typename mpl::make_tuple<typename SuperClass::composed_sequence_t>::type s_composedSequence;

    return Return{ execute(static_cast<const SuperClass*>(this)->m_callables.template get<Indexs>(),s_composedSequence.template get<Indexs>(),i_args) ... };
}
template<typename SuperClass,typename Return, typename ... Types>
template<typename Callable, size_t ... Indexs, typename ... Args>
auto union_function_executor<SuperClass,Return,mpl::type_pack<Types...>>::execute(const Callable& i_callable, const mpl::sequence<Indexs...>&, const tuple<Args...>& i_args) const
{
	return ddk::eval(i_callable,i_args.template get<Indexs>() ...);
}

template<typename Callable, typename ... Callables>
union_function<Callable,Callables...>::union_function(const Callable& i_callable, const Callables& ... i_callables)
: m_callables(i_callable,i_callables...)
{
}
template<typename Callable, typename ... Callables>
union_function<Callable,Callables...>::union_function(const tuple<Callable,Callables...>& i_callables)
: m_callables(i_callables)
{
}
template<typename Callable, typename ... Callables>
union_function<Callable,Callables...>::union_function(tuple<Callable,Callables...>&& i_callables)
: m_callables(std::move(i_callables))
{
}
template<typename Callable, typename ... Callables>
union_function<Callable,Callables...>::union_function(const union_function<Callable,Callables...>& other)
: m_callables(other.m_callables)
{
}
template<typename Callable, typename ... Callables>
union_function<Callable,Callables...>::union_function(union_function<Callable,Callables...>&& other)
: m_callables(std::move(other.m_callables))
{
}

}
}
