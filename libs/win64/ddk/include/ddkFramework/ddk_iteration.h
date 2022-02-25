#pragma once

#include "ddk_iterable.h"
#include "ddk_function.h"
#include "ddk_future.h"

namespace ddk
{

template<typename Iterable, typename Function>
class iteration
{
	template<typename IIterable, typename FFunction>
	friend action_result execute_iteration(iteration<IIterable,FFunction> i_co_iteration);

public:
	iteration(Iterable& i_iterable, const Function& i_try);
	iteration(const iteration&);
	iteration(iteration&&);
	~iteration();

	iteration* operator->();
	const iteration* operator->() const;
	action_result execute();
	action_result execute() const;
	template<typename T>
	future<action_result> attach(T&& i_execContext);
	future<action_result> attach(const detail::this_thread_t&);

private:
	Iterable m_iterable;
	const Function m_try;
	mutable bool m_received;
};

template<typename Iterable, typename Function>
class co_iteration
{
    template<typename IIterable, typename FFunction>
    friend action_result execute_co_iteration(co_iteration<IIterable,FFunction> i_co_iteration);

public:
    co_iteration(Iterable& i_iterable, const Function& i_try);
    co_iteration(const co_iteration&);
    co_iteration(co_iteration&&);
    ~co_iteration();

    co_iteration* operator->();
    const co_iteration* operator->() const;
    action_result execute();
    action_result execute() const;
	template<typename T>
	future<action_result> attach(T&& i_execContext);
	future<action_result> attach(const detail::this_thread_t&);

private:
    Iterable m_iterable;
    const Function m_try;
    mutable bool m_received;
};

}

#include "ddk_iteration.inl"
