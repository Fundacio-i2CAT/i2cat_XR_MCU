
#include "ddk_function_utils.h"

namespace ddk
{

template<typename Callable,typename ... Args>
specialized_callable<Callable,Args...>::specialized_callable(Callable& i_callable,Args ... i_args)
: m_callable(i_callable)
, m_args(std::forward<Args>(i_args)...)
{
}
template<typename Callable,typename ... Args>
template<typename ... Types>
auto specialized_callable<Callable,Args...>::operator()(Types&& ... i_args) const
{
	return private_call(typename mpl::make_sequence<0,mpl::get_num_types<Args...>()>::type{},std::forward<Types>(i_args)...);
}
template<typename Callable,typename ... Args>
template<size_t ... Indexs,typename ... Types>
auto specialized_callable<Callable,Args...>::private_call(const mpl::sequence<Indexs...>&,Types&& ... i_args) const
{
	return ddk::eval(m_callable,m_args.template get<Indexs>() ...,std::forward<Types>(i_args) ...);
}

}
