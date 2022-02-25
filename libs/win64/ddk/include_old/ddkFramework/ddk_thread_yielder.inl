#pragma once


namespace ddk
{
namespace detail
{

template<typename T>
template<typename TT>
typed_yielder_context<T>::typed_yielder_context(TT&& i_value)
{
	m_value.template construct<result_type>(std::forward<TT>(i_value));
}
template<typename T>
typed_yielder_context<T>::typed_yielder_context(const typed_yielder_context& other)
{
	if(other.m_value.empty() == false)
	{
		m_value.template construct<result_type>(other.m_value.template get<result_type>());
	}
}
template<typename T>
typed_yielder_context<T>::typed_yielder_context(typed_yielder_context&& other)
{
	if(other.m_value.empty() == false)
	{
		m_value.template construct<result_type>(other.m_value.template extract<result_type>());
	}
}
template<typename T>
typed_yielder_context<T>::~typed_yielder_context()
{
	m_value.template destroy<result_type>();
}
template<typename T>
TEMPLATE(typename Arg)
REQUIRED(IS_CONSTRUCTIBLE(result_type,Arg))
void typed_yielder_context<T>::insert_value(Arg&& i_value)
{
	m_value.template assign<result_type>(std::forward<Arg>(i_value));
}
template<typename T>
const typename typed_yielder_context<T>::result_type& typed_yielder_context<T>::get_value() const
{
	return m_value.template get<result_type>();
}
template<typename T>
typename typed_yielder_context<T>::result_type& typed_yielder_context<T>::get_value()
{
	return m_value.template get<result_type>();
}

}
}
