
#include "ddk_reference_wrapper.h"
#include "ddk_iterable_exceptions.h"
#include "ddk_iterable_visitor.h"
#include "ddk_forwarding_iterable_value_callable.h"

namespace ddk
{

template<typename Iterable, typename Function>
action_result execute_co_iteration(co_iteration<Iterable,Function> i_co_iteration)
{
    return i_co_iteration.m_iterable.co_iterate(i_co_iteration.m_try);
}

template<typename Iterable, typename Function>
action_result execute_iteration(iteration<Iterable,Function> i_iteration)
{
	return i_iteration.m_iterable.iterate(i_iteration.m_try);
}

template<typename Iterable, typename Function>
iteration<Iterable,Function>::iteration(Iterable& i_iterable, const Function& i_try)
: m_iterable(i_iterable)
, m_try(i_try)
, m_received(false)
{
}
template<typename Iterable, typename Function>
iteration<Iterable,Function>::iteration(const iteration& other)
: m_iterable(other.m_iterable)
, m_try(other.m_try)
, m_received(true)
{
	other.m_received = true;
}
template<typename Iterable, typename Function>
iteration<Iterable,Function>::iteration(iteration&& other)
: m_iterable(std::move(other.m_iterable))
, m_try(std::move(other.m_try))
, m_received(other.m_received)
{
}
template<typename Iterable, typename Function>
iteration<Iterable,Function>::~iteration()
{
	if(m_received == false)
	{
		execute();
	}
}
template<typename Iterable, typename Function>
iteration<Iterable,Function>* iteration<Iterable,Function>::operator->()
{
	return this;
}
template<typename Iterable, typename Function>
const iteration<Iterable,Function>* iteration<Iterable,Function>::operator->() const
{
	return this;
}
template<typename Iterable, typename Function>
action_result iteration<Iterable,Function>::execute()
{
	if constexpr(IS_BASE_OF_ITERABLE_COND(Iterable))
	{
		if(m_iterable != nullptr)
		{
			return m_iterable.iterate(m_try);
		}
		else
		{
			throw iteration_exception{"exeuting empty iteration" };
		}
	}
	else
	{
		typedef ddk::resolved_iterable_action<const Iterable> action;

		try
		{
			ddk::visit_iterator(m_iterable,ddk::forwarding_iterable_value_callable<Function,action>{m_try},action{});
		}
		catch(const ddk::suspend_exception&)
		{
		}

		return {};
	}
}
template<typename Iterable, typename Function>
action_result iteration<Iterable,Function>::execute() const
{
	if constexpr(IS_BASE_OF_ITERABLE_COND(Iterable))
	{
		if(m_iterable != nullptr)
		{
			return m_iterable.iterate(m_try);
		}
		else
		{
			throw iteration_exception{ "exeuting empty iteration" };
		}
	}
	else
	{
		typedef ddk::resolved_iterable_action<const Iterable> action;

		try
		{
			ddk::visit_iterator(m_iterable,ddk::forwarding_iterable_value_callable<Function,action>{m_try},action{});
		}
		catch(const ddk::suspend_exception&)
		{
		}

		return {};
	}
}
template<typename Iterable, typename Function>
template<typename T>
future<action_result> iteration<Iterable,Function>::attach(T&& i_execContext)
{
	shared_reference_wrapper<async_executor<action_result>> res = make_async_executor(make_function(&ddk::execute_co_iteration<Iterable,Function>,*this));

	return res->attach(std::forward<T>(i_execContext));
}
template<typename Iterable, typename Function>
future<action_result> iteration<Iterable,Function>::attach(const detail::this_thread_t&)
{
	return make_async_executor(make_function(&ddk::execute_co_iteration<Iterable,Function>,*this));
}

template<typename Iterable, typename Function>
co_iteration<Iterable,Function>::co_iteration(Iterable& i_iterable, const Function& i_try)
: m_iterable(i_iterable)
, m_try(i_try)
, m_received(false)
{
}
template<typename Iterable, typename Function>
co_iteration<Iterable,Function>::co_iteration(const co_iteration& other)
: m_iterable(other.m_iterable)
, m_try(other.m_try)
, m_received(true)
{
	other.m_received = true;
}
template<typename Iterable, typename Function>
co_iteration<Iterable,Function>::co_iteration(co_iteration&& other)
: m_iterable(std::move(other.m_iterable))
, m_try(std::move(other.m_try))
, m_received(other.m_received)
{
}
template<typename Iterable, typename Function>
co_iteration<Iterable,Function>::~co_iteration()
{
    if(m_received == false)
    {
        execute();
    }
}
template<typename Iterable, typename Function>
co_iteration<Iterable,Function>* co_iteration<Iterable,Function>::operator->()
{
    return this;
}
template<typename Iterable, typename Function>
const co_iteration<Iterable,Function>* co_iteration<Iterable,Function>::operator->() const
{
    return this;
}
template<typename Iterable, typename Function>
action_result co_iteration<Iterable,Function>::execute()
{
	if(m_iterable != nullptr)
    {
        return m_iterable.co_iterate(m_try);
    }
	else
	{
		throw iteration_exception{"Trying to execute empty iteration"};
	}
}
template<typename Iterable, typename Function>
action_result co_iteration<Iterable,Function>::execute() const
{
    if(m_iterable != nullptr)
    {
        return m_iterable.co_iterate(m_try);
    }
	else
	{
		throw iteration_exception{ "Trying to execute empty iteration" };
	}
}
template<typename Iterable, typename Function>
template<typename T>
future<action_result> co_iteration<Iterable,Function>::attach(T&& i_execContext)
{
	distributed_reference_wrapper<async_executor<action_result>> res = make_async_executor(make_function(&ddk::execute_co_iteration<Iterable,Function>,*this));

    return res->attach(std::forward<T>(i_execContext));
}
template<typename Iterable, typename Function>
future<action_result> co_iteration<Iterable,Function>::attach(const detail::this_thread_t&)
{
	return make_async_executor(make_function(&ddk::execute_co_iteration<Iterable,Function>,*this));
}

}
