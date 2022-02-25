#pragma once

#include "ddk_dynamic_visitor.h"

namespace ddk
{
namespace detail
{

template<typename Callable, typename Return, typename TypeInterface>
struct dynamic_callable_impl : public dynamic_visitor<TypeInterface>
{
public:
	typedef Return return_type;

	dynamic_callable_impl(const Callable& i_callable);
	dynamic_callable_impl(const dynamic_callable_impl&) = default;
	dynamic_callable_impl(dynamic_callable_impl&&) = default;

	template<typename ... Args>
	inline Return operator()(Args&& ... i_args) const;
	template<typename ... Args>
	inline Return operator()(Args&& ... i_args);

private:
	Callable m_callable;
};

}

template<typename Return, typename TypeInterface, typename Callable>
inline auto dynamic_callable(const Callable& i_callable);

}

#include "ddk_dynamic_callable.inl"