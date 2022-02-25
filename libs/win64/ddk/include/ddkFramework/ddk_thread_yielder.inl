#pragma once


namespace ddk
{
namespace detail
{

template<typename T>
template<typename TT>
typed_yielder_context<T>::typed_yielder_context(TT&& i_value)
: m_value(std::forward<TT>(i_value))
{
}
template<typename T>
typed_yielder_context<T>::typed_yielder_context(const typed_yielder_context& other)
: m_value(other.m_value)
{
}
template<typename T>
typed_yielder_context<T>::typed_yielder_context(typed_yielder_context&& other)
: m_value(std::move(other.m_value))
{
}
template<typename T>
TEMPLATE(typename Arg)
REQUIRED(IS_CONSTRUCTIBLE(result_type,Arg))
void typed_yielder_context<T>::insert_value(Arg&& i_value)
{
	m_value = std::forward<Arg>(i_value);
}
template<typename T>
const typename typed_yielder_context<T>::result_type& typed_yielder_context<T>::get_value() const
{
	return m_value;
}
template<typename T>
typename typed_yielder_context<T>::result_type& typed_yielder_context<T>::get_value()
{
	return m_value;
}

}
}
