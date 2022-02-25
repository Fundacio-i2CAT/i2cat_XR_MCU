
namespace ddk
{

template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::trivial_union_type::trivial_union_type()
{
}
template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::trivial_union_type::trivial_union_type(const detail::none_t&)
{
}
template<typename Type,typename ... Types>
template<typename TT>
constexpr variadic_union<Type,Types...>::trivial_union_type::trivial_union_type(const  mpl::class_holder<Type>& i_holder,TT&& i_value)
: _local(std::forward<TT>(i_value))
{
}
template<typename Type,typename ... Types>
template<typename T,typename TT>
constexpr variadic_union<Type,Types...>::trivial_union_type::trivial_union_type(const mpl::class_holder<T>& i_holder,TT&& i_value)
: _nested(i_holder,std::forward<TT>(i_value))
{
}

template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::non_trivial_union_type::non_trivial_union_type()
{
}
template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::non_trivial_union_type::non_trivial_union_type(const detail::none_t&)
{
}
template<typename Type,typename ... Types>
template<typename TT>
variadic_union<Type,Types...>::non_trivial_union_type::non_trivial_union_type(const  mpl::class_holder<Type>& i_holder,TT&& i_value)
: _local(std::forward<TT>(i_value))
{
}
template<typename Type,typename ... Types>
template<typename T,typename TT>
variadic_union<Type,Types...>::non_trivial_union_type::non_trivial_union_type(const mpl::class_holder<T>& i_holder,TT&& i_value)
: _nested(i_holder,std::forward<TT>(i_value))
{
}
template<typename Type,typename ... Types>
variadic_union<Type,Types...>::non_trivial_union_type::~non_trivial_union_type()
{
	//already handled by front-end
}

template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::variadic_union()
: variadic_union(typename mpl::static_if<std::is_default_constructible<Type>::value,std::true_type,std::false_type>::type{})
{
}
template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::variadic_union(const detail::none_t&)
{
}
template<typename Type,typename ... Types>
template<typename T, typename TT>
constexpr variadic_union<Type,Types...>::variadic_union(const mpl::class_holder<T>& i_holder, TT&& i_value)
: m_data(i_holder,std::forward<TT>(i_value))
{
}
template<typename Type,typename ... Types>
template<typename T,typename ... Args>
constexpr void variadic_union<Type,Types...>::construct(Args&& ... i_args)
{
	if constexpr(std::is_same<Type,T>::value)
	{
		m_data._local.inplace_construct(std::forward<Args>(i_args)...);
	}
	else
	{
		m_data._nested.template construct<T>(std::forward<Args>(i_args)...);
	}
}
template<typename Type,typename ... Types>
template<typename T,typename ... Args>
constexpr void variadic_union<Type,Types...>::assign(Args&& ... i_args)
{
	if constexpr(std::is_same<Type,T>::value)
	{
		m_data._local.inplace_assign(std::forward<Args>(i_args)...);
	}
	else
	{
		m_data._nested.template assign<T>(std::forward<Args>(i_args)...);
	}
}
template<typename Type,typename ... Types>
template<typename T>
void variadic_union<Type,Types...>::destroy()
{
	if constexpr(std::is_same<Type,T>::value)
	{
		m_data._local.inplace_destroy();
	}
	else
	{
		m_data._nested.template destroy<T>();
	}
}
template<typename Type,typename ... Types>
template<typename T>
constexpr const T& variadic_union<Type,Types...>::get() const
{
	if constexpr(std::is_same<Type,T>::value)
	{
		return m_data._local.get();
	}
	else
	{
		return m_data._nested.template get<T>();
	}
}
template<typename Type,typename ... Types>
template<typename T>
constexpr T& variadic_union<Type,Types...>::get()
{
	if constexpr(std::is_same<Type,T>::value)
	{
		return m_data._local.get();
	}
	else
	{
		return m_data._nested.template get<T>();
	}
}
template<typename Type,typename ... Types>
template<typename T>
T&& variadic_union<Type,Types...>::extract()
{
	if constexpr(std::is_same<Type,T>::value)
	{
		return m_data._local.extract();
	}
	else
	{
		return m_data._nested.template extract<T>();
	}
}
template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::variadic_union(const std::true_type&)
: m_data(mpl::class_holder<Type>{},Type{})
{
}
template<typename Type,typename ... Types>
constexpr variadic_union<Type,Types...>::variadic_union(const std::false_type&)
{
}

}