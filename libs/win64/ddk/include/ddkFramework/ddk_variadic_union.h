#pragma once

#include "ddk_embedded_type.h"
#include "ddk_concepts.h"
#include "ddk_type_concepts.h"
#include "ddk_none.h"

namespace ddk
{

template<typename ... Types>
class variadic_union;

template<>
class variadic_union<>
{
};

template<typename Type,typename ... Types>
class variadic_union<Type,Types...>
{
	union trivial_union_type
	{
		constexpr trivial_union_type();
		constexpr trivial_union_type(const detail::none_t&);
		template<typename TT>
		constexpr trivial_union_type(const  mpl::class_holder<Type>& i_holder,TT&& i_value);
		template<typename T,typename TT>
		constexpr trivial_union_type(const mpl::class_holder<T>& i_holder,TT&& i_value);

		embedded_type<Type> _local;
		variadic_union<Types...> _nested;
	};

	union non_trivial_union_type
	{
		constexpr non_trivial_union_type();
		constexpr non_trivial_union_type(const detail::none_t&);
		template<typename TT>
		non_trivial_union_type(const  mpl::class_holder<Type>& i_holder,TT&& i_value);
		template<typename T,typename TT>
		non_trivial_union_type(const mpl::class_holder<T>& i_holder,TT&& i_value);
		~non_trivial_union_type();

		embedded_type<Type> _local;
		variadic_union<Types...> _nested;
	};

public:
	constexpr variadic_union();
	constexpr variadic_union(const detail::none_t&);
	template<typename T, typename TT>
	constexpr variadic_union(const mpl::class_holder<T>&, TT&& i_value);
	~variadic_union() = default;
	template<typename T,typename ... Args>
	constexpr void construct(Args&& ... i_args);
	template<typename T,typename ... Args>
	constexpr void assign(Args&& ... i_args);
	template<typename T>
	inline void destroy();
	template<typename T>
	constexpr const T& get() const;
	template<typename T>
	constexpr T& get();
	template<typename T>
	inline T&& extract();

private:
	constexpr variadic_union(const std::true_type&);
	constexpr variadic_union(const std::false_type&);

	typedef typename mpl::static_if<mpl::holds_any_type<std::is_trivially_destructible,Type,Types...>(),trivial_union_type,non_trivial_union_type>::type union_type;

	union_type m_data;
};

}

#include "ddk_variadic_union.inl"