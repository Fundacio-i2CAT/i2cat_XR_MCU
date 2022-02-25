#pragma once

#include "ddk_tuple.h"
#include "ddk_tuple_template_helper.h"
#include "ddk_function_template_helper.h"
#include "ddk_tagged_pointer.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_pointer_wrapper.h"
#include "ddk_distribute_from_this.h"

namespace ddk
{
namespace detail
{

template<typename...>
struct function_impl_base;

template<typename T, typename TT>
using function_impl_base_dist_ref = distributed_reference_wrapper<function_impl_base<T, TT>>;
template<typename T, typename TT>
using function_impl_base_const_dist_ref = distributed_reference_wrapper<const function_impl_base<T, TT>>;
template<typename T, typename TT>
using function_impl_base_dist_ptr = distributed_pointer_wrapper<function_impl_base<T, TT>>;
template<typename T, typename TT>
using function_impl_base_const_dist_ptr = distributed_pointer_wrapper<const function_impl_base<T, TT>>;
template<typename T, typename TT>
using function_impl_base_const_lent_ptr = lent_pointer_wrapper<const function_impl_base<T, TT>>;
template<typename Sequence, typename ... Types>
using types_at_indexs = typename mpl::make_tuple<Types...>::template at<Sequence>::type;
template<typename TTypes, typename ... Types>
using place_holder_types = typename mpl::type_pack<Types...>::template at<typename mpl::pos_place_holder<0,TTypes>::type>::type;
template<typename TTypes>
using place_holders_at_indexs = typename mpl::pos_place_holder<0,TTypes>::type;
template<typename TTypes, typename ... Types>
using unresolved_types = typename mpl::type_pack<Types...>::template at<typename place_holders_at_indexs<TTypes>::template at<typename mpl::sequence_place_holder<TTypes>::type>::type>::type;
template<typename TTypes, typename Types>
using unresolved_tuple = typename mpl::type_pack<Types>::template at<typename place_holders_at_indexs<TTypes>::template at<typename mpl::sequence_place_holder<TTypes>::type>::type>::type;
template<typename T>
using forwarded_arg = typename mpl::static_if<std::is_copy_constructible<T>::value,T,typename std::add_rvalue_reference<T>::type>::type;

template<typename Return, typename ... Types>
struct function_impl_base<Return, mpl::type_pack<Types...>> : public distribute_from_this<function_impl_base<Return,mpl::type_pack<Types...>>>
{
    static const size_t s_numTypes = mpl::get_num_types<Types...>();

    template<typename...>
    friend struct function_impl_base;

	template<typename,typename>
	struct specialized_impl;

	template<size_t ... specIndexs, size_t ... notSpecIndexs>
	struct specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>> : function_impl_base<Return, typename mpl::type_pack<Types...>::template at<mpl::sequence<notSpecIndexs...>>::type>
	{
#ifndef _WIN32
		static_assert((std::is_copy_constructible<typename mpl::nth_type_of<specIndexs, Types...>::type>::value && ...), "You cannot specialize non copy constructible arguments");
#endif
        typedef types_at_indexs<mpl::sequence<notSpecIndexs...>,typename mpl::static_if<std::is_copy_constructible<Types>::value,Types,typename std::add_rvalue_reference<Types>::type>::type...> vars_tuple;
        typedef types_at_indexs<mpl::sequence<specIndexs...>,typename std::remove_const<typename std::remove_reference<Types>::type>::type ...> args_tuple;

        specialized_impl() = default;
        template<typename ... Args>
		specialized_impl(const function_impl_base_const_dist_ref<Return,mpl::type_pack<Types...>>& i_object, tuple<Args...>&& i_args);

    private:
		Return operator()(forwarded_arg<typename mpl::nth_type_of<notSpecIndexs,Types...>::type> ... i_args) const override;
        Return apply(const vars_tuple& i_tuple) const override;

        const function_impl_base_const_dist_ref<Return,mpl::type_pack<Types...>> m_object;
		mutable args_tuple m_specArgs;
	};

    typedef tuple<forwarded_arg<Types>...> tuple_args;
	typedef Return return_type;
	typedef mpl::type_pack<Types...> args_type;

	virtual ~function_impl_base() = default;

	template<typename Allocator, typename ... Args>
	function_impl_base_const_dist_ref<Return,unresolved_types<mpl::type_pack<Args...>,Types...>> specialize(const Allocator& i_allocator, Args&& ... args) const;

	virtual Return operator()(forwarded_arg<Types> ... args) const = 0;

private:
    virtual Return apply(const tuple_args& i_tuple) const = 0;
};

template<typename Return,typename ... Types>
using function_base = function_impl_base<Return,mpl::type_pack<Types...>>;

template<typename T,typename ... Types>
using function_base_dist_ref = distributed_reference_wrapper<function_base<T,Types...>>;
template<typename T,typename ... Types>
using function_base_const_dist_ref = distributed_reference_wrapper<const function_base<T,Types...>>;
template<typename T,typename ... Types>
using function_base_dist_ptr = distributed_pointer_wrapper<function_base<T,Types...>>;
template<typename T,typename ... Types>
using function_base_const_dist_ptr = distributed_pointer_wrapper<const function_base<T,Types...>>;
template<typename T,typename ... Types>
using function_base_const_lent_ptr = lent_pointer_wrapper<const function_base<T,Types...>>;


//non static member function case
template<typename ObjectType, typename Return, typename ... Types>
class relative_function_impl : public function_base<Return,Types...>
{
    typedef Return(ObjectType::*NonConstFuncPointerType)(Types...);
    typedef Return(ObjectType::*ConstFuncPointerType)(Types...)const;
    typedef typename mpl::static_if<std::is_const<ObjectType>::value,ConstFuncPointerType,NonConstFuncPointerType>::type FuncPointerType;
    using function_base<Return,Types...>::s_numTypes;
    using typename function_base<Return,Types...>::tuple_args;

public:
	typedef Return return_type;

	constexpr relative_function_impl(ObjectType* i_object, FuncPointerType i_funcPointer);
	constexpr relative_function_impl(const relative_function_impl&) = delete;
	constexpr relative_function_impl(relative_function_impl&& other);

	inline Return inline_eval(forwarded_arg<Types> ... args) const;
	Return operator()(forwarded_arg<Types> ... args) const final;

private:
    Return apply(const tuple_args& i_tuple) const final;
    template<size_t ... Indexs>
    Return apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const;

	ObjectType* m_object;
    FuncPointerType m_funcPointer;
};

//static member function or free function case
template<typename Return, typename ... Types>
class free_function_impl : public function_base<Return,Types...>
{
    typedef Return(*FuncPointerType)(Types...);
    using function_base<Return,Types...>::s_numTypes;
    using typename function_base<Return,Types...>::tuple_args;

public:
	typedef Return return_type;

	constexpr free_function_impl(FuncPointerType i_funcPointer);
	constexpr free_function_impl(free_function_impl&&) = default;

	inline Return inline_eval(forwarded_arg<Types> ... args) const;
	Return operator()(forwarded_arg<Types> ... args) const final;

private:
    Return apply(const tuple_args& i_tuple) const final;
    template<size_t ... Indexs>
    Return apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const;

    FuncPointerType m_funcPointer;
};

//functor (lambdas) case
template<typename T, typename Return, typename ... Types>
class aggregated_functor_impl : public function_base<Return,Types...>
{
    using function_base<Return,Types...>::s_numTypes;
    using typename function_base<Return,Types...>::tuple_args;

public:
	typedef Return return_type;

	constexpr aggregated_functor_impl(const T& i_functor);
	constexpr aggregated_functor_impl(T&& i_functor);

	inline Return inline_eval(forwarded_arg<Types> ... args) const;
	Return operator()(forwarded_arg<Types> ... args) const final;

private:
    Return apply(const tuple_args& i_tuple) const final;
    template<size_t ... Indexs>
    Return apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const;

	mutable T m_functor;
};

template<typename,typename,typename>
struct get_resolved_functor;
template<typename Functor, typename Return,typename ... Types>
struct get_resolved_functor<Functor,Return,mpl::type_pack<Types...>>
{
	typedef aggregated_functor_impl<Functor,Return,Types...> type;
};

template<typename Callable, typename Return,typename Type>
using resolved_functor = typename get_resolved_functor<Callable,Return,Type>::type;

template<typename Callable>
using resolved_functor_impl = resolved_functor<Callable,typename mpl::aqcuire_callable_return_type<Callable>::type,typename mpl::aqcuire_callable_args_type<Callable>::type>;

template<typename Return,typename ... Types>
class inherited_functor_impl : public function_base<Return,Types...>
{
	using function_base<Return,Types...>::s_numTypes;
	using typename function_base<Return,Types...>::tuple_args;

public:
	typedef Return return_type;

	constexpr inherited_functor_impl() = default;
	constexpr inherited_functor_impl(const inherited_functor_impl&) = default;
	constexpr inherited_functor_impl(inherited_functor_impl&&) = default;

	inline Return inline_eval(forwarded_arg<Types> ... args) const;

private:
	Return apply(const tuple_args& i_tuple) const final;
	template<size_t ... Indexs>
	Return apply(const mpl::sequence<Indexs...>&,const tuple_args& i_tuple) const;
};

template<typename T, typename Return, typename ... Types>
inline constexpr bool is_base_of_inherited_functor_impl = std::is_base_of<inherited_functor_impl<Return,Types...>,T>::value;

}
}

#include "ddk_function_impl.inl"
