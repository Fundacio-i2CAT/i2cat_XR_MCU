
#include "ddk_embedded_type.h"
#include "ddk_dynamic_callable.h"

namespace ddk
{
namespace detail
{

template<typename Callable,typename,typename>
struct resolve_callable_return_type;

template<typename Callable, size_t ... Indexs, typename ... Types>
struct resolve_callable_return_type<Callable,mpl::sequence<Indexs...>,mpl::type_pack<Types...>>
{
	typedef decltype(std::declval<typename std::remove_reference<Callable>::type>()(std::declval<mpl::type_pack<Types...>::template nth_type<mpl::index_to_index<Indexs,0>>>()...)) type;
};

template<typename FinalVisitorType, typename VisitorType>
bool __expand_type_visitor_layout()
{
	typedef typename VisitorType::type visitor_type;
	typedef typename VisitorType::type_interface type_interface;
	static const FinalVisitorType* s_finalType = reinterpret_cast<const FinalVisitorType*>(0xFFFF);
	static const typed_dynamic_visitor<visitor_type>* s_partialType = s_finalType;
	static const rtti::TypeInfo typeInfo = rtti::type_info<visitor_type,type_interface>();

	dynamic_visitor<type_interface>::add_dynamic_visitor(typeInfo,reinterpret_cast<size_t>(s_partialType) - reinterpret_cast<size_t>(s_finalType));

	return true;
}

template<typename MultiVisitor,typename Visitor,typename Type>
const bool dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>::__register_type_visitor = __expand_type_visitor_layout<MultiVisitor,dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>>();

template<typename MultiVisitor,typename Visitor,typename Type>
void dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>::visit(const Type& i_value)
{
	static_cast<const MultiVisitor*>(this)->typed_visit(i_value);
}
template<typename MultiVisitor,typename Visitor,typename Type>
void dynamic_multi_visitor_base<MultiVisitor,Visitor,Type>::visit(const Type& i_value) const
{
	static_cast<const MultiVisitor*>(this)->typed_visit(i_value);
}

}

template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Value,typename ... Values>
dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>::dynamic_multi_visitor(Visitor& i_visitor,const inherited_value<Value>& i_value,const inherited_value<Values>& ... i_pendingValues)
: m_visitor(i_visitor)
, m_value(i_value)
, m_pendingValues(i_pendingValues...)
{
	static const bool __value = (detail::dynamic_multi_visitor_base<dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>,Visitor,Types>::__register_type_visitor && ...);
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Value,typename ... Values>
function<typename Visitor::return_type(ResolvedTypes...)> dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>::visit() const
{
	m_value.template visit<typename Visitor::type_interface>(*this);

	return m_resolvedFunction;
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Value,typename ... Values>
template<typename T>
void dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>::typed_visit(T&& i_resolvedValue) const
{
	typedef typename mpl::make_sequence<0,mpl::get_num_types<ResolvedTypes...>()>::type indexs_resolved;
	typedef typename mpl::make_sequence<0,mpl::get_num_types<Values...>()>::type indexs_to_resolve;

	typed_visit(indexs_resolved{},indexs_to_resolve{},std::forward<T>(i_resolvedValue));
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes,typename Value,typename ... Values>
template<size_t ... IndexsResolved,size_t ... IndexsToResolve,typename T>
void dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>,Value,Values...>::typed_visit(const mpl::sequence<IndexsResolved...>&, const mpl::sequence<IndexsToResolve...>&,T&& i_resolvedValue) const
{
	typedef typename std::add_lvalue_reference<T>::type resolved_type;
	dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...,embedded_type<resolved_type>>,Values...> tmpVisitor(m_visitor,m_pendingValues.template get<IndexsToResolve>() ...);

	const function<return_type(ResolvedTypes...,embedded_type<resolved_type>)> partialFunction = tmpVisitor.visit();

	m_resolvedFunction = partialFunction(mpl::place_holder<IndexsResolved>{}...,std::forward<T>(i_resolvedValue));
}

template<typename Visitor,typename ... Types,typename ... ResolvedTypes>
dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>>::dynamic_multi_visitor(Visitor& i_visitor)
: m_visitor(i_visitor)
{
}
template<typename Visitor,typename ... Types,typename ... ResolvedTypes>
function<typename Visitor::return_type(ResolvedTypes...)> dynamic_multi_visitor<Visitor,mpl::type_pack<Types...>,mpl::type_pack<ResolvedTypes...>>::visit() const
{
	return make_function([thisVisitor = std::move(m_visitor)](ResolvedTypes ... i_values) mutable -> return_type 
	{ 
		if constexpr (std::is_same<return_type,void>::value)
		{
			thisVisitor(*i_values ...);
		}
		else
		{
			return thisVisitor(*i_values ...);
		}
	});
}

TEMPLATE(typename TypeInterface, typename Callable,typename ... Values)
REQUIRED(IS_NOT_INHERITED_VALUE(Callable),IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
auto visit(Callable&& i_callable,const Values& ... i_values)
{
	static const bool s_typeExpanded = rtti::inherited_type_expansion<TypeInterface>;
	typedef rtti::inherited_type_list<TypeInterface> inherited_type_pack;
	typedef typename detail::resolve_callable_return_type<Callable,typename mpl::make_sequence<0,mpl::num_types<Values...>>::type,inherited_type_pack>::type return_type;
	auto _visitor = dynamic_callable<return_type,TypeInterface>(i_callable);

	dynamic_multi_visitor<decltype(_visitor),inherited_type_pack,mpl::type_pack<>,typename Values::value_type...> multiVisitor(_visitor,i_values ...);

	const function<return_type()> resolvedFunc = multiVisitor.visit();

	if constexpr(std::is_same<return_type,void>::value)
	{
		eval(resolvedFunc);
	}
	else
	{
		return eval(resolvedFunc);
	}
}

TEMPLATE(typename Return,typename TypeInterface,typename Callable,typename ... Values)
REQUIRED(IS_NOT_INHERITED_VALUE(Callable),IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
auto visit(Callable&& i_callable,const Values& ... i_values)
{
	static const bool s_typeExpanded = rtti::inherited_type_expansion<TypeInterface>;
	typedef rtti::inherited_type_list<TypeInterface> inherited_type_pack;
	auto _visitor = dynamic_callable<Return,TypeInterface>(i_callable);

	dynamic_multi_visitor<decltype(_visitor),inherited_type_pack,mpl::type_pack<>,typename Values::value_type...> multiVisitor(_visitor,i_values ...);

	const function<Return()> resolvedFunc = multiVisitor.visit();

	if constexpr (std::is_same<Return,void>::value)
	{
		eval(resolvedFunc);
	}
	else
	{
		return eval(resolvedFunc);
	}
}

TEMPLATE(typename Callable, typename TypeInterface, typename ... Values)
REQUIRED(IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
auto visit(const Values& ... i_values)
{
	static const bool s_typeExpanded = rtti::inherited_type_expansion<TypeInterface>;
	typedef rtti::inherited_type_list<TypeInterface> inherited_type_pack;
	typedef typename detail::resolve_callable_return_type<Callable,typename mpl::make_sequence<0,mpl::num_types<Values...>>::type,inherited_type_pack>::type return_type;

	const auto _visitor = dynamic_callable<return_type,TypeInterface>(Callable{});

	dynamic_multi_visitor<decltype(_visitor),inherited_type_pack,mpl::type_pack<>,typename Values::value_type...> multiVisitor(_visitor,i_values ...);

	const function<return_type()> resolvedFunc = multiVisitor.visit();

	if constexpr(std::is_same<return_type,void>::value)
	{
		eval(resolvedFunc);
	}
	else
	{
		return eval(resolvedFunc);
	}
}

TEMPLATE(typename Return,typename Callable,typename TypeInterface,typename ... Values)
REQUIRED(IS_NUMBER_OF_ARGS_GREATER_OR_EQUAL(1,Values...),IS_INHERITED_VALUE(Values)...)
auto visit(const Values& ... i_values)
{
	static const bool s_typeExpanded = rtti::inherited_type_expansion<TypeInterface>;
	typedef rtti::inherited_type_list<TypeInterface> inherited_type_pack;

	const auto _visitor = dynamic_callable<Return,TypeInterface>(Callable{});

	dynamic_multi_visitor<decltype(_visitor),inherited_type_pack,mpl::type_pack<>,typename Values::value_type...> multiVisitor(_visitor,i_values ...);

	const function<Return()> resolvedFunc = multiVisitor.visit();

	if constexpr(std::is_same<Return,void>::value)
	{
		eval(resolvedFunc);
	}
	else
	{
		return eval(resolvedFunc);
	}
}

}
