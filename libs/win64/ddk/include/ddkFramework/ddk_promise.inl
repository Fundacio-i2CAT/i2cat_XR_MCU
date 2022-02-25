#pragma once

#include "ddk_reference_wrapper.h"
#include "ddk_future.inl"
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename T>
promise<T>::promise()
{
	detail::private_async_state<T>* sharedState = new detail::private_async_state<T>();

	m_sharedState = as_shared_reference(sharedState,tagged_pointer<decltype(sharedState->m_refCounter)>(&sharedState->m_refCounter, ReferenceAllocationType::Embedded),nullptr);
}
template<typename T>
promise<T>::promise(const promise<T>& other)
: m_sharedState(other.m_sharedState)
{
}
template<typename T>
promise<T>& promise<T>::operator=(const promise<T>& other)
{
	m_sharedState = other.m_sharedState;

	return *this;
}
template<typename T>
void promise<T>::set_value(sink_type i_value)
{
	m_sharedState->set_value(i_value);
}
template<typename T>
void promise<T>::set_exception(const async_exception& i_exception)
{
	m_sharedState->set_exception(i_exception);
}
template<typename T>
future<T> promise<T>::get_future() const
{
	return m_sharedState;
}
template<typename T>
void promise<T>::wait() const
{
	return m_sharedState->wait();
}
template<typename T>
void promise<T>::wait_for(unsigned int i_period) const
{
	return m_sharedState->wait_for(i_period);
}
template<typename T>
bool promise<T>::ready() const
{
	return m_sharedState->ready();
}
template<typename T>
void promise<T>::signal() const
{
	return m_sharedState->signal();
}

}
