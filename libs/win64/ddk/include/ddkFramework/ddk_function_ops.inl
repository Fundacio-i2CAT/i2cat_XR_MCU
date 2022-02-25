
#include <cmath>

namespace ddk
{

template<typename Return,typename ... Types,typename Allocator,typename FunctionImpl>
detail::function_impl<Return(Types...),Allocator,FunctionImpl> deduce_function(const detail::function_impl<Return(Types...),Allocator,FunctionImpl>& i_function)
{
	return i_function;
}
TEMPLATE(typename Callable)
REQUIRED(IS_CALLABLE(Callable))
auto deduce_function(Callable&& i_callable)
{
	return make_function(i_callable);
}

template<typename T>
auto sum(const std::initializer_list<T>& i_values)
{
	T res = 0;

	for(const auto& value : i_values)
	{
		res += value;
	}

	return res;
}
template<typename T>
auto prod(const std::initializer_list<T>& i_values)
{
	T res = 1;

	for(const auto& value : i_values)
	{
		res *= value;
	}

	return res;
}

}