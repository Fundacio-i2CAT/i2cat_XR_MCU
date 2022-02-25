
#include "ddk_function.h"
#include "ddk_callable.h"

namespace ddk
{
namespace detail
{

template<typename Return, typename Callable, typename ... ResolvedTypes, typename Variant, typename ... Variants>
multi_visitor<Return,Callable,tuple<ResolvedTypes...>,Variant,Variants...>::multi_visitor(const Callable& i_transform, Variant& i_variant,Variants& ... i_variants)
: m_transform(i_transform)
, m_toResolveVariant(i_variant)
, m_pendingVariants(i_variants...)
{
}
template<typename Return, typename Callable, typename ... ResolvedTypes, typename Variant, typename ... Variants>
template<size_t ... IndexsResolved, size_t ... IndexsToResolve, typename T>
function<Return(ResolvedTypes...)> multi_visitor<Return,Callable,tuple<ResolvedTypes...>,Variant,Variants...>::visit(const mpl::sequence<IndexsResolved...>&, const mpl::sequence<IndexsToResolve...>&, T&& i_value) const
{
	typedef typename std::add_lvalue_reference<T>::type resolved_type;
	multi_visitor<Return,Callable,tuple<ResolvedTypes...,embedded_type<resolved_type>>,Variants...> subTransform(m_transform,m_pendingVariants.template get<IndexsToResolve>() ...);

	const function<Return(ResolvedTypes...,embedded_type<resolved_type>)> partialTransform = subTransform.visit();

	return partialTransform(mpl::place_holder<IndexsResolved>{}...,std::forward<T>(i_value));
}
template<typename Return, typename Callable, typename ... ResolvedTypes, typename Variant, typename ... Variants>
template<typename T>
function<Return(ResolvedTypes...)> multi_visitor<Return,Callable,tuple<ResolvedTypes...>,Variant,Variants...>::operator()(T&& i_value) const
{
	typedef typename mpl::make_sequence<0,mpl::get_num_types<ResolvedTypes...>()>::type indexs_resolved;
	typedef typename mpl::make_sequence<0,mpl::get_num_types<Variants...>()>::type indexs_to_resolved;

	return visit(indexs_resolved{},indexs_to_resolved{},std::forward<T>(i_value));
}
template<typename Return, typename Callable, typename ... ResolvedTypes, typename Variant, typename ... Variants>
function<Return(ResolvedTypes...)> multi_visitor<Return,Callable,tuple<ResolvedTypes...>,Variant,Variants...>::visit() const
{
	return m_toResolveVariant.visit(*this);
}

template<typename Return,typename Callable, typename ... ResolvedTypes>
multi_visitor<Return,Callable,tuple<ResolvedTypes...>>::multi_visitor(const Callable& i_transform)
: m_transform(i_transform)
{
}
template<typename Return,typename Callable, typename ... ResolvedTypes>
function<Return(ResolvedTypes...)> multi_visitor<Return,Callable,tuple<ResolvedTypes...>>::visit() const
{
	return make_function([thisTransform = std::move(m_transform)](ResolvedTypes ... i_values) -> Return 
	{
		if constexpr ( std::is_same<void,Return>::value)
		{
			thisTransform(*i_values ...);
		}
		else
		{
			return thisTransform(*i_values ...);
		}
	});
}

}

TEMPLATE(typename Return, typename Callable,typename ... Variants)
REQUIRED(IS_NUMBER_OF_ARGS_GREATER(1,Variants...),IS_VARIANT(Variants)...)
constexpr auto visit(Variants&& ... i_variants)
{
	typedef typename std::remove_reference<decltype(deduce_fixed_callable<Return>(std::declval<Callable>())) >::type callable_t;

	detail::multi_visitor<Return,callable_t,tuple<>,typename mpl::static_if<std::is_lvalue_reference<Variants>::value,Variants,const Variants>::type...> multiVisitor(callable_t{},std::forward<Variants>(i_variants)...);

	const function<Return()> resolvedFunction = multiVisitor.visit();

	if constexpr (std::is_same<void,Return>::value)
	{
		eval(resolvedFunction);
	}
	else
	{
		return eval(resolvedFunction);
	}
}
TEMPLATE(typename Callable,typename ... Variants)
REQUIRED(IS_NUMBER_OF_ARGS_GREATER(1,Variants...),IS_VARIANT(Variants)...)
constexpr auto visit(Variants&& ... i_variants)
{
	typedef decltype(std::declval<Callable>()(std::declval<typename std::remove_reference<Variants>::type::type_pack::template nth_type<0>>()...)) return_type;
	typedef typename std::remove_reference<decltype(deduce_fixed_callable<return_type>(std::declval<Callable>())) >::type callable_t;

	detail::multi_visitor<return_type,callable_t,tuple<>,typename mpl::static_if<std::is_lvalue_reference<Variants>::value,Variants,const Variants>::type...> multiVisitor(callable_t{},std::forward<Variants>(i_variants)...);

	const function<return_type()> resolvedFunction = multiVisitor.visit();

	if constexpr(std::is_same<void,return_type>::value)
	{
		eval(resolvedFunction);
	}
	else
	{
		return eval(resolvedFunction);
	}
}

TEMPLATE(typename Return, typename Callable,typename ... Variants)
REQUIRED(IS_NUMBER_OF_ARGS_GREATER(1,Variants...),IS_NOT_VARIANT(Callable),IS_VARIANT(Variants)...)
constexpr auto visit(Callable&& i_callable,Variants&& ... i_variants)
{
    typedef typename std::remove_reference<decltype(deduce_fixed_callable<Return>(i_callable))>::type callable_t;

	detail::multi_visitor<Return,callable_t,tuple<>,typename mpl::static_if<std::is_lvalue_reference<Variants>::value,Variants,const Variants>::type...> multiVisitor(deduce_fixed_callable<Return>(i_callable),std::forward<Variants>(i_variants)...);

	const function<Return()> resolvedFunction = multiVisitor.visit();

	if constexpr(std::is_same<void,Return>::value)
	{
		eval(resolvedFunction);
	}
	else
	{
		return eval(resolvedFunction);
	}
}
TEMPLATE(typename Callable,typename ... Variants)
REQUIRED(IS_NUMBER_OF_ARGS_GREATER(1,Variants...),IS_NOT_VARIANT(Callable),IS_VARIANT(Variants)...)
constexpr auto visit(Callable&& i_callable,Variants&& ... i_variants)
{
	typedef decltype(std::declval<Callable>()(std::declval<typename std::remove_reference<Variants>::type::type_pack::template nth_type<0>>()...)) return_type;
	typedef typename std::remove_reference<decltype(deduce_fixed_callable<return_type>(i_callable))>::type callable_t;

	detail::multi_visitor<return_type,callable_t,tuple<>,typename mpl::static_if<std::is_lvalue_reference<Variants>::value,Variants,const Variants>::type...> multiVisitor(deduce_fixed_callable<return_type>(i_callable),std::forward<Variants>(i_variants)...);

	const function<return_type()> resolvedFunction = multiVisitor.visit();

	if constexpr(std::is_same<void,return_type>::value)
	{
		eval(resolvedFunction);
	}
	else
	{
		return eval(resolvedFunction);
	}
}

}
