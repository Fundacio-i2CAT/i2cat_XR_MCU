#pragma once

#include "ddk_template_helper.h"

namespace ddk
{

template<typename ... Callables>
class callable : public Callables ...
{
public:
	struct callable_tag;
	using Callables::operator()...;

	callable() = default;
	template<typename ... CCallables>
	callable(CCallables&& ... i_callables);
};
template<typename ... Callables>
callable(Callables&& ... i_callables) -> callable<mpl::remove_qualifiers<Callables>...>;

template<typename Return, typename ... Callables>
class fixed_return_callable : public Callables ...
{
public:
	typedef Return return_type;
	struct callable_tag;
	using Callables::operator()...;

	fixed_return_callable() = default;
	template<typename ... CCallables>
	fixed_return_callable(CCallables&& ... i_callables);
};

template<typename Callable>
inline auto deduce_callable(Callable&& i_callable);
template<typename Return, typename Callable>
inline auto deduce_fixed_callable(Callable&& i_callable);
template<typename Callable>
inline auto deduce_callable(const callable<Callable>& i_callable);
template<typename Return, typename Callable>
inline auto deduce_fixed_callable(const fixed_return_callable<Return,Callable>& i_callable);

}

#include "ddk_callable.inl"