#pragma once

#include "ddk_tuple.h"
#include "ddk_function_template_helper.h"
#include "ddk_tuple_template_helper.h"
#include "ddk_function_arguments.h"
#include "ddk_function_impl.h"

namespace ddk
{
namespace detail
{

template<size_t,typename ...>
struct sequence_composer;

template<size_t Index>
struct sequence_composer<Index>
{
    typedef mpl::type_pack<> type;
};

template<size_t Index, typename CallableArgs, typename ... CallablesArgs>
struct sequence_composer<Index,CallableArgs,CallablesArgs...>
{
private:
    typedef typename mpl::make_sequence<Index,Index+CallableArgs::size()>::type curr_sequence;
    typedef typename sequence_composer<Index+CallableArgs::size(),CallablesArgs...>::type next_composed_sequence;

public:
    typedef typename mpl::merge_type_packs<mpl::type_pack<curr_sequence>,next_composed_sequence>::type type;
};

template<typename,typename,typename>
class union_function_executor;

template<typename SuperClass,typename Return, typename ... Types>
class union_function_executor<SuperClass,Return,mpl::type_pack<Types...>> : public inherited_functor_impl<Return,Types...>
{
public:
    union_function_executor() = default;
    Return operator()(forwarded_arg<Types> ... i_args) const final override;

private:
    template<size_t ... Indexs, typename ... Args>
    inline Return execute(const mpl::sequence<Indexs...>&, const tuple<Args...>& i_args) const;
    template<typename Callable, size_t ... Indexs, typename ... Args>
    inline auto execute(const Callable& i_callable, const mpl::sequence<Indexs...>&, const tuple<Args...>& i_args) const;
};

template<typename Callable, typename ... Callables>
class union_function : public union_function_executor<union_function<Callable,Callables...>,function_arguments<typename mpl::aqcuire_callable_return_type<Callable>::type, typename mpl::aqcuire_callable_return_type<Callables>::type ...>,typename mpl::merge_type_packs<typename mpl::aqcuire_callable_args_type<Callable>::type,typename mpl::aqcuire_callable_args_type<Callables>::type ...>::type>
{
    static const size_t s_num_callables = mpl::get_num_types<Callables...>() + 1;
    typedef typename sequence_composer<0,typename mpl::aqcuire_callable_args_type<Callable>::type,typename mpl::aqcuire_callable_args_type<Callables>::type...>::type composed_sequence_t;

    template<typename,typename,typename>
    friend class union_function_executor;

public:
    typedef function_arguments<typename mpl::aqcuire_callable_return_type<Callable>::type, typename mpl::aqcuire_callable_return_type<Callables>::type ...> callable_return_type;
    typedef typename mpl::merge_type_packs<typename mpl::aqcuire_callable_args_type<Callable>::type,typename mpl::aqcuire_callable_args_type<Callables>::type ...>::type callable_args_type;

    union_function(const Callable& i_callable, const Callables& ... i_callables);
    union_function(const tuple<Callable,Callables...>& i_callables);
    union_function(tuple<Callable,Callables...>&& i_callables);
    union_function(const union_function<Callable,Callables...>& other);
    union_function(union_function<Callable,Callables...>&& other);

private:
	const tuple<Callable,Callables...> m_callables;
};

}
}

#include "ddk_union_function.inl"
