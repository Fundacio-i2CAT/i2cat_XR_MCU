#pragma once

#include "ddk_arena.h"
#include "ddk_concepts.h"
#include "ddk_function_concepts.h"
#include "ddk_function_impl.h"
#include "ddk_function_view.h"
#include "ddk_tuple_template_helper.h"
#include "ddk_function_arguments.h"
#include "ddk_allocator_concepts.h"
#include "ddk_system_allocator.h"
#include "ddk_global_allocators.h"

namespace ddk
{
namespace detail
{

template<typename,typename,typename>
struct get_resolved_function;
template<typename Return,typename ... Types,typename Allocator>
struct get_resolved_function<Return,mpl::type_pack<Types...>,Allocator>
{
    typedef function<Return(Types...),Allocator> type;
};

template<typename Return, typename Type, typename Allocator = system_allocator>
using resolved_function = typename get_resolved_function<Return,Type,typename std::enable_if<concepts::is_allocator<Allocator>::value,Allocator>::type>::type;

template<typename Callable, typename Allocator = system_allocator>
using resolved_callable = resolved_function<typename mpl::aqcuire_callable_return_type<mpl::remove_qualifiers<Callable>>::type,typename mpl::aqcuire_callable_args_type<mpl::remove_qualifiers<Callable>>::type,Allocator>;

template<typename Callable, typename Allocator, typename ... Args>
using resolved_spec_callable = resolved_function<typename mpl::aqcuire_callable_return_type<mpl::remove_qualifiers<Callable>>::type,detail::unresolved_tuple<mpl::type_pack<Args...>,typename mpl::aqcuire_callable_args_type<mpl::remove_qualifiers<Callable>>::type>,Allocator>;

template<typename Arg, typename T>
using resolved_return_type = typename std::enable_if<is_function_argument<Arg>::value==false,T>::type;

template<typename,typename,typename>
class function_impl;

template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
class function_impl<Return(Types...),Allocator,FunctionImpl>
{
    template<typename,typename,typename>
    friend class function_impl;
    template<typename RReturn,typename ... TTypes,typename AAllocator, typename FFunctionImpl>
    friend function_view<RReturn(TTypes...)> _lend(const function_impl<RReturn(TTypes...),AAllocator,FFunctionImpl>&);
    friend std::true_type _is_function(const function_impl*);

public:
	struct callable_tag;
	typedef Return return_type;
    typedef mpl::type_pack<Types...> args_type;

	function_impl();
    function_impl(std::nullptr_t);
    function_impl(const function_impl& other) = default;
    function_impl(function_impl&& other) = default;
    template<typename AAllocator>
    function_impl(const function_impl<Return(Types...),AAllocator,FunctionImpl>& other);
    template<typename AAllocator>
    function_impl(function_impl<Return(Types...),AAllocator,FunctionImpl>&& other);
	TEMPLATE(typename T)
	REQUIRES(IS_CALLABLE_NOT_FUNCTION(T,Types...))
	function_impl(T&& functor, const Allocator& i_allocator = Allocator());
    template<typename T>
    function_impl(T *pRef, Return(T::*call)(Types...), const Allocator& i_allocator = Allocator());
    template<typename T>
    function_impl(const T *pRef, Return(T::*call)(Types...)const, const Allocator& i_allocator = Allocator());
    function_impl(Return(*call)(Types...), const Allocator& i_allocator = Allocator());
    function_impl& operator=(const function_impl& other) = default;
    function_impl& operator=(function_impl&& other) = default;
    function_impl& operator=(std::nullptr_t);
    inline bool operator==(std::nullptr_t) const;
    inline bool operator!=(std::nullptr_t) const;
	TEMPLATE(typename ... Args)
    REQUIRES(IS_CONSTRUCTIBLE(Types,Args)...)
	inline Return inline_eval(Args&& ... args) const;
    TEMPLATE(typename ... Args)
    REQUIRES(IS_CONSTRUCTIBLE(Types,Args)...)
    inline Return inline_eval(const function_arguments<Args...>& i_args) const;
	template<typename ... Args>
    inline NO_DISCARD_RETURN resolved_function<Return,detail::unresolved_types<mpl::type_pack<Args...>,Types...>,Allocator> operator()(Args&& ... args) const;

private:
    template<size_t ... Indexs, typename ... Args>
    inline Return eval_arguments(const mpl::sequence<Indexs...>&, const function_arguments<Args...>& i_args) const;

protected:
    FunctionImpl m_functionImpl;
	fixed_size_allocate_or<Allocator> m_allocator;
};

}

template<typename Return, typename ... Types>
using function_impl_const_ptr = detail::function_base_const_dist_ptr<Return,Types...>;

template<typename Return, typename ... Types, typename Allocator>
class function<Return(Types...),Allocator> : public detail::function_impl<Return(Types...),Allocator,function_impl_const_ptr<Return,Types...>>
{
    typedef detail::function_impl<Return(Types...),Allocator,function_impl_const_ptr<Return,Types...>> function_base_t;

public:
    using function_base_t::function_base_t;

    function& operator=(std::nullptr_t);
};

namespace mpl
{

template<typename Return, typename ... Types, typename Allocator, typename FunctionImpl>
struct aqcuire_callable_return_type<detail::function_impl<Return(Types...),Allocator,FunctionImpl>>
{
	typedef Return type;
};
template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl>
struct aqcuire_callable_args_type<detail::function_impl<Return(Types...),Allocator,FunctionImpl>>
{
    typedef mpl::type_pack<Types...> type;
};
template<typename Return, typename ... Types, typename Allocator>
struct aqcuire_callable_return_type<function<Return(Types...),Allocator>>
{
	typedef Return type;
};
template<typename Return,typename ... Types,typename Allocator>
struct aqcuire_callable_args_type<function<Return(Types...),Allocator>>
{
    typedef mpl::type_pack<Types...> type;
};

}

//template<typename Return, typename ... Types>
//using curried_function = typename yame::mpl::curry_function<yame::ytl::function<Return(Types...)>>::type;

}

#include "ddk_function.inl"

#include "ddk_function_utils.h"
#include "ddk_function_ops.h"