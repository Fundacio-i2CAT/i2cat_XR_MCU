
namespace ddk
{
namespace detail
{

template<typename Callable,typename Return,typename TypeInterface>
dynamic_callable_impl<Callable,Return,TypeInterface>::dynamic_callable_impl(const Callable& i_callable)
: m_callable(i_callable)
{
}
template<typename Callable,typename Return,typename TypeInterface>
template<typename ... Args>
Return dynamic_callable_impl<Callable,Return,TypeInterface>::operator()(Args&& ... i_args) const
{
	if constexpr (std::is_same<Return,void>::value)
	{
		eval(m_callable,std::forward<Args>(i_args)...);
	}
	else
	{
		return eval(m_callable,std::forward<Args>(i_args)...);
	}
}
template<typename Callable,typename Return,typename TypeInterface>
template<typename ... Args>
Return dynamic_callable_impl<Callable,Return,TypeInterface>::operator()(Args&& ... i_args)
{
	if constexpr(std::is_same<Return,void>::value)
	{
		eval(m_callable,std::forward<Args>(i_args)...);
	}
	else
	{
		return eval(m_callable,std::forward<Args>(i_args)...);
	}
}

}

template<typename Return,typename TypeInterface,typename Callable>
auto dynamic_callable(const Callable& i_callable)
{
	return detail::dynamic_callable_impl<Callable,Return,TypeInterface>{ i_callable };
}

}