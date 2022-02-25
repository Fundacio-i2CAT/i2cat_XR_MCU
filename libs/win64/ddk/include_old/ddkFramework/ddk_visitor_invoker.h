#pragma once

#include "ddk_template_helper.h"

namespace ddk
{
namespace detail
{

template<typename Return, typename ... Types>
class variant_visitor_invoker
{
    static const size_t _numTypes = mpl::get_num_types<Types...>();

public:
    template<typename visitor, size_t TypePos>
	constexpr static Return inner_invoke(visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::cref_type ref_type;

		if constexpr(std::is_same<void,Return>::value)
		{
			_visitor.template operator() < TypePos > (std::forward<ref_type>(var.template get<TypePos>()));
		}
		else
		{
			return _visitor.template operator()<TypePos>(std::forward<ref_type>(var.template get<TypePos>()));
		}
	}
    template<typename visitor, size_t TypePos>
	constexpr static Return inner_invoke(const visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::cref_type ref_type;

		if constexpr(std::is_same<void,Return>::value)
		{
			_visitor.template operator()<TypePos>(std::forward<ref_type>(var.template get<TypePos>()));
		}
		else
		{
			return _visitor.template operator() < TypePos > (std::forward<ref_type>(var.template get<TypePos>()));
		}
	}
    template<typename visitor, size_t TypePos>
	constexpr static Return inner_invoke(visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::ref_type ref_type;

		if constexpr (std::is_same<void,Return>::value)
		{
			_visitor.template operator()<TypePos>(std::forward<ref_type>(var.template get<TypePos>()));
		}
		else
		{
			return _visitor.template operator() < TypePos > (std::forward<ref_type>(var.template get<TypePos>()));
		}
	}
    template<typename visitor, size_t TypePos>
	constexpr static Return inner_invoke(const visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::ref_type ref_type;

		if constexpr(std::is_same<void,Return>::value)
		{
			_visitor.template operator() < TypePos > (std::forward<ref_type>(var.template get<TypePos>()));
		}
		else
		{
			return _visitor.template operator()<TypePos>(std::forward<ref_type>(var.template get<TypePos>()));
		}
	}
    template<typename visitor, size_t TypePos>
	constexpr static Return inner_invoke(visitor& _visitor, variant_impl<Types...>&& var)
	{
		if constexpr(std::is_same<void,Return>::value)
		{
			_visitor.template operator() < TypePos > (std::move(var).template extract<TypePos>());
		}
		else
		{
			return _visitor.template operator()<TypePos>(std::move(var).template extract<TypePos>());
		}
	}
    template<typename visitor, size_t TypePos>
    constexpr static Return inner_invoke(const visitor& _visitor, variant_impl<Types...>&& var)
	{
		if constexpr(std::is_same<void,Return>::value)
		{
			_visitor.template operator() < TypePos > (std::move(var).template extract<TypePos>());
		}
		else
		{
			return _visitor.template operator()<TypePos>(std::move(var).template extract<TypePos>());
		}
	}
    template<typename visitor, size_t TypePos>
	constexpr static Return outer_invoke(visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::cref_type ref_type;

		if constexpr(std::is_same<void,Return>::value)
		{
			_visitor.operator()(std::forward<ref_type>(var.template get<TypePos>()));
		}
		else
		{
			return _visitor.operator()(std::forward<ref_type>(var.template get<TypePos>()));
		}
	}
    template<typename visitor, size_t TypePos>
	constexpr static Return outer_invoke(const visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::cref_type ref_type;

		if constexpr(std::is_same<void,Return>::value)
		{
			_visitor.operator()(std::forward<ref_type>(var.template get<TypePos>()));
		}
		else
		{
			return _visitor.operator()(std::forward<ref_type>(var.template get<TypePos>()));
		}
	}
    template<typename visitor, size_t TypePos>
	constexpr static Return outer_invoke(visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::ref_type ref_type;

		if constexpr(std::is_same<void,Return>::value)
		{
			_visitor.operator()(std::forward<ref_type>(var.template get<TypePos>()));
		}
		else
		{
			return _visitor.operator()(std::forward<ref_type>(var.template get<TypePos>()));
		}
	}
    template<typename visitor, size_t TypePos>
	constexpr static Return outer_invoke(const visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef typename mpl::nth_type_of<TypePos,Types...>::type CurrType;
		typedef typename embedded_type<CurrType>::ref_type ref_type;

		if constexpr(std::is_same<void,Return>::value)
		{
			_visitor.operator()(std::forward<ref_type>(var.template get<TypePos>()));
		}
		else
		{
			return _visitor.operator()(std::forward<ref_type>(var.template get<TypePos>()));
		}
	}
    template<typename visitor, size_t TypePos>
	constexpr static Return outer_invoke(visitor& _visitor, variant_impl<Types...>&& var)
	{
		if constexpr(std::is_same<void,Return>::value)
		{
			_visitor.operator()(var.template extract<TypePos>());
		}
		else
		{
			return _visitor.operator()(var.template extract<TypePos>());
		}
	}
    template<typename visitor, size_t TypePos>
	constexpr static Return outer_invoke(const visitor& _visitor, variant_impl<Types...>&& var)
	{
		if constexpr(std::is_same<void,Return>::value)
		{
			_visitor.operator()(var.template extract<TypePos>());
		}
		else
		{
			return _visitor.operator()(var.template extract<TypePos>());
		}
	}

public:
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(const visitor&, const variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,var);
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor, var);
		}
	}
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(visitor&, const variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,var);
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor,var);
		}
	}
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(const visitor&, variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,var);
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor,var);
		}
	}
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(visitor&, variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,var);
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor,var);
		}
	}
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, variant_impl<Types...>&& var)
	{
		typedef Return(*funcType)(const visitor&, variant_impl<Types...>&&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,std::move(var));
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor,std::move(var));
		}
	}
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return outer_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, variant_impl<Types...>&& var)
	{
		typedef Return(*funcType)(visitor&, variant_impl<Types...>&&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::outer_invoke<visitor,TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,std::move(var));
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor,std::move(var));
		}
	}
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(const visitor&, const variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor,TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,var);
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor,var);
		}
	}
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, const variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(visitor&, const variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor, TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,var);
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor,var);
		}
	}
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(const visitor&, variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor,TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,var);
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor,var);
		}
	}
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, variant_impl<Types...>& var)
	{
		typedef Return(*funcType)(visitor&, variant_impl<Types...>&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor, TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,var);
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor,var);
		}
	}
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, const visitor& _visitor, variant_impl<Types...>&& var)
	{
		typedef Return(*funcType)(const visitor&, variant_impl<Types...>&&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor,TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,std::move(var));
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor,std::move(var));
		}
	}
    template<typename visitor, size_t ... TypeIndexs>
	constexpr static Return inner_invoker(const mpl::sequence<TypeIndexs...>&, visitor& _visitor, variant_impl<Types...>&& var)
	{
		typedef Return(*funcType)(visitor&, variant_impl<Types...>&&);
		typedef variant_visitor_invoker<Return, Types...> this_variant;

		const funcType funcTable[_numTypes] = { &this_variant::inner_invoke<visitor,TypeIndexs> ... };

		if constexpr(std::is_same<void,Return>::value)
		{
			(*funcTable[var.m_currentType])(_visitor,std::move(var));
		}
		else
		{
			return (*funcTable[var.m_currentType])(_visitor,std::move(var));
		}
	}
};

}
}
