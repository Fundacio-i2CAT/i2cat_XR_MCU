#pragma once

#include "ddk_function_template_helper.h"
#include "ddk_function_arguments.h"
#include "ddk_function_impl.h"

namespace ddk
{
namespace detail
{

template<typename,typename,typename>
class intersection_function_executor;

template<typename SuperClass,typename Return, typename ... Types>
class intersection_function_executor<SuperClass,Return,mpl::type_pack<Types...>> : public inherited_functor_impl<Return,Types...>
{
public:
    intersection_function_executor() = default;
    Return operator()(forwarded_arg<Types> ... i_args) const final override;

private:
    template<size_t ... Indexs>
    inline Return execute(const mpl::sequence<Indexs...>&, forwarded_arg<Types> ... i_args) const;
};

template<typename Callable, typename ... Callables>
class intersection_function : public intersection_function_executor<intersection_function<Callable,Callables...>,function_arguments<typename mpl::aqcuire_callable_return_type<Callable>::type, typename mpl::aqcuire_callable_return_type<Callables>::type ...>,typename mpl::aqcuire_callable_args_type<Callable>::type>
{
    static const size_t s_num_callables = 1 + mpl::get_num_types<Callables...>();

    template<typename,typename,typename>
    friend class intersection_function_executor;

public:
    typedef function_arguments<typename mpl::aqcuire_callable_return_type<Callable>::type, typename mpl::aqcuire_callable_return_type<Callables>::type ...> callable_return_type;
    typedef typename mpl::aqcuire_callable_args_type<Callable>::type callable_args_type;

    intersection_function(const Callable& i_callable, const Callables& ... i_callables);
    intersection_function(const tuple<Callable,Callables...>& i_callables);
    intersection_function(tuple<Callable,Callables...>&& i_callables);
    intersection_function(const intersection_function<Callable,Callables...>& other);
    intersection_function(intersection_function<Callable,Callables...>&& other);

    template<size_t Index>
    const mpl::nth_type_of_t<Index,Callable,Callables...>& get_callable() const;

private:
	const tuple<Callable,Callables...> m_callables;
};

template<typename...>
struct resolve_intersection;

template<typename CallableA, typename CallableB>
struct resolve_intersection<CallableA,CallableB>
{
    typedef intersection_function<CallableA,CallableB> type;
};
template<typename CallableA, typename ... CallablesB>
struct resolve_intersection<CallableA,intersection_function<CallablesB...>>
{
    typedef intersection_function<CallableA,CallablesB...> type;
};
template<typename ... CallablesA, typename CallableB>
struct resolve_intersection<intersection_function<CallablesA...>,CallableB>
{
    typedef intersection_function<CallablesA...,CallableB> type;
};
template<typename ... CallablesA, typename ... CallablesB>
struct resolve_intersection<intersection_function<CallablesA...>,intersection_function<CallablesB...>>
{
    typedef intersection_function<CallablesA...,CallablesB...> type;
};

template<typename ... Callables>
using resolved_intersection = typename resolve_intersection<Callables...>::type;

}
}

#include "ddk_intersection_function.inl"
