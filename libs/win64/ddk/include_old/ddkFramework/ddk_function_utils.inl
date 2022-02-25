
#include "ddk_composed_function.h"

namespace ddk
{
namespace detail
{

template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl>
function_view<Return(Types...)> _lend(const function_impl<Return(Types...),Allocator,FunctionImpl>& i_function)
{
	return lend(i_function.m_functionImpl);
}

}

template<typename Object,typename Return,typename ... Types>
constexpr detail::relative_function_impl<Object,Return,Types...> make_member_function(Object* i_object,Return(Object::*i_funcPtr)(Types...))
{
	return std::move(detail::relative_function_impl<Object,Return,Types...>(i_object,i_funcPtr));
}
template<typename Object,typename Return,typename ... Types>
constexpr detail::relative_function_impl<const Object,Return,Types...> make_member_function(const Object* i_object,Return(Object::*i_funcPtr)(Types...)const)
{
	return std::move(detail::relative_function_impl<const Object,Return,Types...>(i_object,i_funcPtr));
}
template<typename Return,typename ... Types>
constexpr detail::free_function_impl<Return,Types...> make_free_function(Return(*i_funcPtr)(Types...))
{
	return std::move(detail::free_function_impl<Return,Types...>(i_funcPtr));
}
TEMPLATE(typename Functor)
REQUIRED(IS_CLASS(Functor),IS_CALLABLE(Functor))
constexpr detail::resolved_functor_impl<Functor> make_functor_function(Functor&& i_functor)
{
	return { std::forward<Functor>(i_functor) };
}

template<typename Object, typename Return, typename ... Types>
function<Return(Types...)> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...))
{
    return function<Return(Types...)>(i_object, i_funcPtr);
}
template<typename Object, typename Return, typename ... Types>
function<Return(Types...)> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Types...)const)
{
    return function<Return(Types...)>(i_object,i_funcPtr);
}
template<typename Return, typename ... Types>
function<Return(Types...)> make_function(Return(*i_funcPtr)(Types...))
{
    return function<Return(Types...)>(i_funcPtr);
}
TEMPLATE(typename Functor)
REQUIRED(IS_CLASS(Functor),IS_CALLABLE(Functor))
detail::resolved_callable<Functor> make_function(Functor&& i_functor)
{
    typedef detail::resolved_callable<Functor> function_type;

    return function_type(std::forward<Functor>(i_functor));
}
TEMPLATE(typename Object, typename Return, typename ... Types, typename Allocator)
REQUIRED(IS_ALLOCATOR(Allocator))
function<Return(Types...),Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Types...), const Allocator& i_allocator)
{
    return function<Return(Types...),Allocator>(i_object, i_funcPtr);
}
TEMPLATE(typename Object, typename Return, typename ... Types, typename Allocator)
REQUIRED(IS_ALLOCATOR(Allocator))
function<Return(Types...),Allocator> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Types...)const, const Allocator& i_allocator)
{
    return function<Return(Types...),Allocator>(i_object,i_funcPtr);
}
TEMPLATE(typename Return, typename ... Types, typename Allocator)
REQUIRED(IS_ALLOCATOR(Allocator))
function<Return(Types...),Allocator> make_function(Return(*i_funcPtr)(Types...), const Allocator& i_allocator)
{
    return function<Return(Types...),Allocator>(i_funcPtr);
}
TEMPLATE(typename Functor, typename Allocator)
REQUIRED(IS_CLASS(Functor),IS_CALLABLE(Functor),IS_ALLOCATOR(Allocator))
detail::resolved_callable<Functor,Allocator> make_function(Functor&& i_functor, const Allocator& i_allocator)
{
    typedef detail::resolved_callable<Functor,Allocator> function_type;

    return function_type(std::forward<Functor>(i_functor),i_allocator);
}
TEMPLATE(typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRED(IS_NOT_ALLOCATOR(Arg))
detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>() == mpl::get_num_types<Args...>(), "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_object, i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
TEMPLATE(typename Object, typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRED(IS_NOT_ALLOCATOR(Arg))
detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::get_num_types<Types...>() == mpl::get_num_types<Args...>(), "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_object,i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
TEMPLATE(typename Return, typename Type, typename ... Types, typename Arg, typename ... Args)
REQUIRED(IS_NOT_ALLOCATOR(Arg))
detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>> make_function(Return(*i_funcPtr)(Type,Types...), Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::num_types<Types...> == mpl::num_types<Args...>, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...)> res(i_funcPtr);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
TEMPLATE(typename Functor, typename Arg, typename ... Args)
REQUIRED(IS_CLASS(Functor),IS_CALLABLE(Functor),IS_NOT_ALLOCATOR(Arg))
detail::resolved_spec_callable<Functor,system_allocator,Arg,Args...> make_function(Functor&& i_functor, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::aqcuire_callable_args_type<Functor>::type::size() == mpl::num_types<Arg,Args...>, "Unconsistent number of arguments with number of types");

    typedef detail::resolved_callable<Functor> function_type;

    const function_type res(std::forward<Functor>(i_functor));

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>,Allocator> make_function(Object* i_object, Return(Object::*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::num_types<Types...> == mpl::num_types<Args...>, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...),Allocator> res(i_object, i_funcPtr, i_allocator);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Object, typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>,Allocator> make_function(const Object* i_object, Return(Object::*i_funcPtr)(Type,Types...)const, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::num_types<Types...> == mpl::num_types<Args...>, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...),Allocator> res(i_object,i_funcPtr,i_allocator);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
template<typename Return, typename Type, typename ... Types, typename Allocator, typename Arg, typename ... Args>
detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Arg,Args...>,Type,Types...>,Allocator> make_function(Return(*i_funcPtr)(Type,Types...), const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::num_types<Types...> == mpl::num_types<Args...>, "Unconsistent number of arguments with number of types");

    const function<Return(Type,Types...),Allocator> res(i_funcPtr,i_allocator);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}
TEMPLATE(typename Functor, typename Allocator, typename Arg, typename ... Args)
REQUIRED(IS_CLASS(Functor),IS_CALLABLE(Functor))
detail::resolved_spec_callable<Functor,Allocator,Arg,Args...> make_function(Functor&& i_functor, const Allocator& i_allocator, Arg&& i_arg, Args&& ... i_args)
{
	static_assert(mpl::aqcuire_callable_args_type<Functor>::type::size() == mpl::get_num_types<Arg,Args...>(), "Unconsistent number of arguments with number of types");

    typedef detail::resolved_callable<Functor,Allocator> function_type;

    const function_type res(std::forward<Functor>(i_functor),i_allocator);

	return res(std::forward<Arg>(i_arg),std::forward<Args>(i_args) ...);
}

template<typename Return, typename Allocator, typename FunctionImpl>
Return eval(const detail::function_impl<Return(),Allocator,FunctionImpl>& i_function)
{
    if constexpr (std::is_same<Return,void>::value)
    {
        i_function.inline_eval();
    }
    else
    {
        return i_function.inline_eval();
    }
}
TEMPLATE(typename Return, typename ... Types, typename Allocator,typename FunctionImpl, typename ... Args)
REQUIRED(is_function_argument<Args>::value==false ...)
Return eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function, Args&& ... i_args)
{
    if constexpr (std::is_same<Return,void>::value)
    {
        i_function.inline_eval(std::forward<Args>(i_args) ...);
    }
    else
    {
        return i_function.inline_eval(std::forward<Args>(i_args) ...);
    }
}
template<typename Return, typename ... Types, typename Allocator,typename FunctionImpl, typename ... Args>
Return eval(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function, const function_arguments<Args...>& i_args)
{
    if constexpr (std::is_same<Return,void>::value)
    {
        i_function.inline_eval(i_args);
    }
    else
    {
        return i_function.inline_eval(i_args);
    }
}
TEMPLATE(typename Function,typename ... Args)
REQUIRED(IS_NOT_FUNCTION(Function),IS_CALLABLE(Function,Args...))
auto eval(Function&& i_function,Args&& ... i_args)
{
	if constexpr (std::is_same<void,decltype(std::declval<Function>().operator()(std::declval<Args>() ...))>::value)
	{
		i_function(std::forward<Args>(i_args) ...);
	}
	else
	{
		return i_function(std::forward<Args>(i_args) ...);
	}
}

template<typename Return,typename Allocator,typename FunctionImpl>
Return eval_unsafe(const detail::function_impl<Return(),Allocator,FunctionImpl>& i_function)
{
	if constexpr(std::is_same<Return,void>::value)
	{
		i_function.inline_eval();
	}
	else
	{
		return i_function.inline_eval();
	}
}
TEMPLATE(typename Return,typename ... Types, typename Allocator,typename FunctionImpl,typename ... Args)
REQUIRED(is_function_argumenttion_arguments<Arg>::value==false ...)
Return eval_unsafe(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,Args&& ... i_args)
{
	if constexpr(std::is_same<Return,void>::value)
	{
		i_function.inline_eval(std::forward<Args>(i_args) ...);
	}
	else
	{
		return i_function.inline_eval(std::forward<Args>(i_args) ...);
	}
}
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
Return eval_unsafe(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,const function_arguments<Args...>& i_args)
{
	if constexpr(std::is_same<Return,void>::value)
	{
		i_function.inline_eval(std::forward<Args>(i_args) ...);
	}
	else
	{
		return i_function.inline_eval(std::forward<Args>(i_args) ...);
	}
}

template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Args...>,Types...>,Allocator> specialize(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,Args&& ... i_args)
{
	return i_function(std::forward<Args>(i_args)...);
}
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl,typename ... Args>
detail::resolved_function<Return,detail::unresolved_types<mpl::type_pack<Args...>,Types...>,Allocator> specialize(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function,const function_arguments<Args...>& i_args)
{
	return i_function(std::forward<Args>(i_args)...);
}
TEMPLATE(typename Functor,typename ... Args)
REQUIRED(IS_CALLABLE(Functor))
specialized_callable<typename std::remove_reference<Functor>::type,Args...> specialize(Functor&& i_functor,Args&& ... i_args)
{
	return specialized_callable<typename std::remove_reference<Functor>::type,Args...>(std::forward<Functor>(i_functor),std::forward<Args>(i_args)...);
}

template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl>
function_view<Return(Types...)> lend(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function)
{
	return _lend(i_function);
}

template<typename ... Callables>
detail::intersection_function<Callables...> make_intersection(const Callables& ... i_callables)
{
    return detail::intersection_function<Callables...>(i_callables...);
}

template<typename ... Callables>
detail::union_function<Callables...> make_union(const Callables& ... i_callables)
{
    return detail::intersection_function<Callables...>(i_callables...);
}

template<typename ReturnDst, typename ... TypesDst, typename ReturnSrc, typename ... TypesSrc>
detail::composed_function<ReturnDst(TypesDst...),ReturnSrc(TypesSrc...)> make_composition(const function<ReturnDst(TypesDst...)>& i_fuscDst, const function<ReturnSrc(TypesSrc...)>& i_funcSrc)
{
    return detail::composed_function<ReturnDst(TypesDst...),ReturnSrc(TypesSrc...)>(i_fuscDst,i_funcSrc);
}

TEMPLATE(typename ... Functions)
REQUIRED(IS_CALLABLE(Functions)...)
detail::intersection_function<Functions...> fusion(const Functions& ... i_functions)
{
    return { i_functions ... };
}

TEMPLATE(typename ... Functions)
REQUIRED(IS_CALLABLE(Functions)...)
detail::union_function<Functions...> concat(const Functions& ... i_functions)
{
    return { i_functions ...};
}

}

template<typename ReturnA, typename ... TypesA, typename ReturnB, typename ... TypesB>
ddk::function<ReturnA(TypesB...)> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::function<ReturnB(TypesB...)>& i_rhs)
{
    return ddk::make_composition(i_lhs,i_rhs);
}
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
ddk::detail::resolved_function<ReturnA,typename ddk::detail::intersection_function<CallablesB...>::callable_args_type> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::detail::intersection_function<CallablesB...>& i_rhs)
{
    return ddk::make_composition(i_lhs,ddk::detail::resolved_function<typename ddk::detail::intersection_function<CallablesB...>::callable_return_type,typename ddk::detail::intersection_function<CallablesB...>::callable_args_type>(i_rhs));
}
template<typename ReturnA, typename ... TypesA, typename ... CallablesB>
ddk::detail::resolved_function<ReturnA,typename ddk::detail::union_function<CallablesB...>::callable_args_type> operator<<=(const ddk::function<ReturnA(TypesA...)>& i_lhs, const ddk::detail::union_function<CallablesB...>& i_rhs)
{
    return ddk::make_composition(i_lhs,ddk::detail::resolved_function<typename ddk::detail::union_function<CallablesB...>::callable_return_type,typename ddk::detail::union_function<CallablesB...>::callable_args_type>(i_rhs));
}
