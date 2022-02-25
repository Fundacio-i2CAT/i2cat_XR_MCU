
namespace ddk
{

template<typename ... Callables>
template<typename ... CCallables>
callable<Callables...>::callable(CCallables&& ... i_callables)
: Callables(std::forward<CCallables>(i_callables))...
{
}

template<typename Return, typename ... Callables>
template<typename ... CCallables>
fixed_return_callable<Return,Callables...>::fixed_return_callable(CCallables&& ... i_callables)
: Callables(std::forward<CCallables>(i_callables))...
{
}

template<typename Callable>
auto deduce_callable(Callable&& i_callable)
{
	return callable(std::forward<Callable>(i_callable));
}
template<typename Return,typename Callable>
auto deduce_fixed_callable(Callable&& i_callable)
{
	return fixed_return_callable<Return,mpl::remove_qualifiers<Callable>>(std::forward<Callable>(i_callable));
}
template<typename Callable>
auto deduce_callable(const callable<Callable>& i_callable)
{
	return i_callable;
}
template<typename Return, typename Callable>
auto deduce_fixed_callable(const fixed_return_callable<Return,Callable>& i_callable)
{
	return i_callable;
}

}