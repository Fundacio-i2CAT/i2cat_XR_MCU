#pragma once

#include "ddk_async_exceptions.h"
#include "ddk_reference_wrapper.h"
#include "ddk_sync_executor.h"

namespace ddk
{

template<typename T>
future<T>::future(future&& other)
: m_sharedState(std::move(other.m_sharedState))
{
}
template<typename T>
future<T>::future(const detail::private_async_state_dist_ptr<T>& i_sharedState)
: m_sharedState(i_sharedState)
{
}
template<typename T>
future<T>::future(detail::private_async_state_dist_ptr<T>&& i_sharedState)
: m_sharedState(std::move(i_sharedState))
{
}
template<typename T>
template<typename TT>
future<T>::future(distributed_reference_wrapper<TT> i_executor,...)
: future(i_executor->as_future())
{
}
template<typename T>
future<T>& future<T>::operator=(future<T>&& other)
{
	m_sharedState = std::move(other.m_sharedState);

	return *this;
}
template<typename T>
future<T>::operator bool() const
{
	return m_sharedState != nullptr;
}
template<typename T>
bool future<T>::valid() const
{
	return m_sharedState != nullptr;
}
template<typename T>
bool future<T>::ready() const
{
	if(m_sharedState)
	{
		return m_sharedState->ready();
	}
	else
	{
		return false;
	}
}
template<typename T>
bool future<T>::empty() const
{
	return m_sharedState == nullptr;
}
template<typename T>
typename future<T>::const_reference future<T>::get_value() const
{
	if(m_sharedState)
	{
		return m_sharedState->get_value();
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
T future<T>::extract_value()
{
	if(m_sharedState)
	{
		embedded_type<T> res = m_sharedState->extract_value();

		return res.extract();
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
typename future<T>::cancel_result future<T>::cancel()
{
	if (m_sharedState)
	{
		const auto cancelRes = m_sharedState->cancel();

		if(cancelRes)
		{
			m_sharedState = nullptr;
		}

		return cancelRes;
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
void future<T>::detach()
{
	m_sharedState = nullptr;
}
template<typename T>
void future<T>::wait() const
{
	if(m_sharedState)
	{
		return m_sharedState->wait();
	}
}
template<typename T>
void future<T>::wait_for(unsigned int i_period) const
{
	if(m_sharedState)
	{
		return m_sharedState->wait_for(i_period);
	}
}
template<typename T>
template<typename TT>
future<TT> future<T>::then(const function<TT(const_reference)>& i_continuation) &&
{
	if(detail::private_async_state_dist_ptr<T> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		auto executor = make_async_executor(make_function([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				eval(i_continuation,acquiredFuture.extract_value());
			}
			else
			{
				return eval(i_continuation,acquiredFuture.extract_value());
			}
		}));

		if(async_base_dist_ptr asyncExecutor = sharedState->get_aync_execution())
		{
			future<TT> res = executor -> attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return executor->as_future();
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
template<typename TT, typename TTT>
future<TT> future<T>::then_on(const function<TT(const_reference)>& i_continuation, TTT&& i_execContext) &&
{
	if(detail::private_async_state_dist_ptr<T> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		auto executor = make_async_executor(make_function([acquiredFuture = std::move(*this),i_continuation,acquiredExecContext = std::forward<TTT>(i_execContext)]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				future<TT> nestedFuture = ddk::async(i_continuation(acquiredFuture.extract_value())) -> attach(std::forward<TTT>(acquiredExecContext));

				nestedFuture.wait();
			}
			else
			{
				future<TT> nestedFuture = ddk::async(i_continuation(acquiredFuture.extract_value())) -> attach(std::forward<TTT>(acquiredExecContext));

				return nestedFuture.extract_value();
			}
		}));

		if(async_base_dist_ptr asyncExecutor = sharedState->get_aync_execution())
		{
			future<TT> res = executor->attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return executor->as_future();
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
template<typename TT, typename TTT>
future<TT> future<T>::async(const function<TT(const_reference)>& i_continuation,TTT&& i_execContext) &&
{
	if(m_sharedState)
	{
		return make_async_executor(make_function([acquiredFuture = std::move(*this),i_continuation]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				eval(i_continuation,acquiredFuture.extract_value());
			}
			else
			{
				return eval(i_continuation,acquiredFuture.extract_value());
			}
		})) -> attach(std::forward<TTT>(i_execContext));
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
future<T> future<T>::on_error(const function<void(const async_error&)>& i_onError) &&
{
	if(detail::private_async_state_dist_ptr<T> sharedState = m_sharedState)
	{
		const unsigned char currDepth = m_depth;

		auto executor = make_async_executor(make_function([acquiredFuture = std::move(*this),i_onError]() mutable
		{
			try
			{
				if constexpr(std::is_same<T,void>::value)
				{
					acquiredFuture.extract_value();
				}
				else
				{
					return acquiredFuture.extract_value();
				}
			}
			catch(const async_exception& i_excp)
			{
				if(i_excp.acquired() == false)
				{
					eval(i_onError,i_excp.as_error());
				}

				throw async_exception{ i_excp.what(),i_excp.get_code(),true };
			}
		}));
		
		if(async_base_dist_ptr asyncExecutor = sharedState->get_aync_execution())
		{
			future<T> res = executor->attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return executor->as_future();
		}
	}

	throw future_exception("Accessing empty future");
}

template<typename T>
template<typename TT>
shared_future<T>::shared_future(shared_reference_wrapper<TT> i_executor,...)
: future<T>(i_executor)
{
}
template<typename T>
shared_future<T>::shared_future(const shared_future& i_future)
: future<T>(i_future.m_sharedState)
{
}
template<typename T>
shared_future<T>::shared_future(shared_future&& i_future)
: future<T>(std::move(i_future.m_sharedState))
{
}
template<typename T>
shared_future<T>::shared_future(future<T>&& i_future)
: future<T>(std::move(i_future))
{
}
template<typename T>
shared_future<T>& shared_future<T>::operator=(const shared_future& other)
{
	this->m_sharedState = other.m_sharedState;

	return *this;
}
template<typename T>
shared_future<T>& shared_future<T>::operator=(shared_future&& other)
{
	this->m_sharedState = std::move(other.m_sharedState);

	return *this;
}
template<typename T>
template<typename TT>
shared_future<TT> shared_future<T>::then(const function<TT(const_reference)>& i_continuation) const
{
	if(detail::private_async_state_dist_ptr<T> sharedState = this->m_sharedState)
	{
		const unsigned char currDepth = this->m_depth;

		auto executor = make_async_executor(make_function([acquiredFuture = *this,i_continuation]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				eval(i_continuation,acquiredFuture.get_value());
			}
			else
			{
				return eval(i_continuation,acquiredFuture.get_value());
			}
		}));

		if(async_base_dist_ptr asyncExecutor = sharedState->get_aync_execution())
		{
			future<TT> res = executor->attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return executor->as_future();
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
template<typename TT,typename TTT>
shared_future<TT> shared_future<T>::then_on(const function<TT(const_reference)>& i_continuation,TTT&& i_execContext) const
{
	if(detail::private_async_state_dist_ptr<T> sharedState = this->m_sharedState)
	{
		const unsigned char currDepth = this->m_depth;

		auto executor = make_async_executor(make_function([acquiredFuture = *this,i_continuation,acquiredExecContext = std::forward<TTT>(i_execContext)]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				shared_future<TT> nestedFuture = ddk::async(i_continuation(acquiredFuture.extract_value()))->attach(std::forward<TTT>(acquiredExecContext));

				nestedFuture.wait();
			}
			else
			{
				shared_future<TT> nestedFuture = ddk::async(i_continuation(acquiredFuture.extract_value()))->attach(std::forward<TTT>(acquiredExecContext));

				return nestedFuture.extract_value();
			}
		}));
			
		if(async_base_dist_ptr asyncExecutor = sharedState->get_aync_execution())
		{
			future<TT> res = executor->attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return executor->as_future();
		}
	}

	throw future_exception("Accessing empty future");
}
template<typename T>
template<typename TT,typename TTT>
shared_future<TT> shared_future<T>::async(const function<TT(const_reference)>& i_continuation,TTT&& i_execContext) const
{
	if(this->m_sharedState)
	{
		return make_async_executor(make_function([acquiredFuture = *this,i_continuation]() mutable
		{
			if constexpr(std::is_same<TT,void>::value)
			{
				eval(i_continuation,acquiredFuture.extract_value());
			}
			else
			{
				return eval(i_continuation,acquiredFuture.extract_value());
			}
		}))->attach(std::forward<TTT>(i_execContext));
	}
	else
	{
		throw future_exception("Accessing empty future");
	}
}
template<typename T>
shared_future<T> shared_future<T>::on_error(const function<void(const async_error&)>& i_onError) const
{
	if(detail::private_async_state_dist_ptr<T> sharedState = this->m_sharedState)
	{
		const unsigned char currDepth = this->m_depth;

		auto executor = make_async_executor(make_function([acquiredFuture = *this,i_onError]() mutable
		{
			try
			{
				if constexpr(std::is_same<T,void>::value)
				{
					acquiredFuture.extract_value();
				}
				else
				{
					return acquiredFuture.extract_value();
				}
			}
			catch(const async_exception& i_excp)
			{
				if(i_excp.acquired() == false)
				{
					eval(i_onError,i_excp.as_error());
				}

				throw async_exception{ i_excp.what(),i_excp.get_code(),true };
			}
		}));
			
		if(async_base_dist_ptr asyncExecutor = sharedState->get_aync_execution())
		{
			future<T> res = executor->attach(asyncExecutor->get_execution_context(),currDepth);

			res.m_depth = currDepth + 1;

			return std::move(res);
		}
		else
		{
			return executor->as_future();
		}
	}

	throw future_exception("Accessing empty future");
}

}
