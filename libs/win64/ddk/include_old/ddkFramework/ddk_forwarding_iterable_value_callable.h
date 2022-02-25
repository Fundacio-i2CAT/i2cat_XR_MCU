#pragma once

namespace ddk
{

template<typename Function,typename Action>
class forwarding_iterable_value_callable
{
public:
	template<typename FFunction>
	forwarding_iterable_value_callable(FFunction&& i_function);
	template<typename T>
	inline Action operator()(T&& i_value) const;

private:
	Function m_function;
};

}

#include "ddk_forwarding_iterable_value_callable.inl"