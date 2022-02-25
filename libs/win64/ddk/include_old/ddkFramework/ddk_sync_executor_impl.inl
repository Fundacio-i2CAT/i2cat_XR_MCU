
#include "ddk_thread_impl.h"
#include "ddk_ucontext.h"
#include "ddk_async_exceptions.h"

namespace ddk
{
namespace detail
{

template<typename Return>
deferred_executor<Return>::deferred_executor()
: m_state(ExecutorState::Idle)
{
}
template<typename Return>
typename deferred_executor<Return>::start_result deferred_executor<Return>::execute(const sink_type& i_sink, const ddk::function<Return()>& i_callable)
{
	if(i_callable != nullptr)
	{
		if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Executing))
		{
			s_execContext.start([=]()
			{
				try
				{
					if constexpr (std::is_same<void,Return>::value)
					{
						eval_unsafe(i_callable);

						while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

						if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
						{
							eval_unsafe(i_sink,_void);
						}

					}
					else
					{
						sink_reference res = eval_unsafe(i_callable);

						while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

						if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
						{
							eval_unsafe(i_sink,res);
						}
					}

					ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed);
						
					return;
				}
				catch(const async_exception& i_excp)
				{
					eval_unsafe(i_sink,i_excp);
				}
				catch(const std::exception& i_excp)
				{
					eval_unsafe(i_sink,async_exception{ i_excp.what() });
				}
				catch(...)
				{
					eval_unsafe(i_sink,async_exception{ "Unkwon exception" });
				}

				//since there has been an exception,if executing pass it to cancelled
				ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
			});
		}

		return make_result<start_result>(m_state.get());
	}
	else
	{
		return make_result<start_result>(ExecutorState::Idle);
	}
}
template<typename Return>
typename deferred_executor<Return>::cancel_result deferred_executor<Return>::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelling))
	{
		if(i_cancelFunc != nullptr && eval_unsafe(i_cancelFunc))
		{
			m_state = ExecutorState::Cancelled;

			return ddk::success;
		}
		else
		{
			m_state = ExecutorState::Executing;

			std::this_thread::yield();

			return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
		}
	}
	else
	{
		return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
	}
}
template<typename Return>
executor_context_lent_ptr deferred_executor<Return>::get_execution_context()
{
	return lend(s_execContext);
}
template<typename Return>
executor_context_const_lent_ptr deferred_executor<Return>::get_execution_context() const
{
	return lend(s_execContext);
}
template<typename Return>
bool deferred_executor<Return>::pending() const
{
	return m_state.get() == ExecutorState::Idle || m_state.get() == ExecutorState::Pending;
}

template<typename Return>
fiber_executor<Return>::fiber_executor(fiber i_fiber)
: m_execContext(std::move(i_fiber))
{
}
template<typename Return>
typename fiber_executor<Return>::start_result fiber_executor<Return>::execute(const sink_type& i_sink, const ddk::function<Return()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(StartErrorCode::StartNoCallable);
	}
	else
	{
		if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Executing))
		{
			m_execContext.start([=]()
			{
				try
				{
					if constexpr(std::is_same<void,Return>::value)
					{
						eval_unsafe(i_callable);

						while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

						if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
						{
							eval_unsafe(i_sink,_void);
						}
					}
					else
					{
						sink_reference res = eval_unsafe(i_callable);

						while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

						if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
						{
							eval_unsafe(i_sink,res);
						}
					}

					return;
				}
				catch(const async_exception& i_excp)
				{
					eval_unsafe(i_sink,i_excp);
				}
				catch(const std::exception& i_excp)
				{
					eval_unsafe(i_sink,async_exception{ i_excp.what() });
				}
				catch(...)
				{
					eval_unsafe(i_sink,async_exception{ "Unkwon exception" });
				}

				while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

				//since there has been an exception,if executing pass it to cancelled
				ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
			});

			return make_result<start_result>(ExecutorState::Executing);
		}
		else
		{
			sink_reference res = eval_unsafe(i_callable);

			while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

			if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
			{
				eval_unsafe(i_sink,res);
			}

			return make_result<start_result>(m_state.get());
		}
	}
}
template<typename Return>
typename fiber_executor<Return>::cancel_result fiber_executor<Return>::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Cancelling))
	{
		if (i_cancelFunc != nullptr && eval_unsafe(i_cancelFunc))
		{
			m_state = ExecutorState::Cancelled;

			return ddk::success;
		}
		else
		{
			m_state = ExecutorState::Executing;

			std::this_thread::yield();

			return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
		}
	}
	else
	{
		return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
	}
}
template<typename Return>
executor_context_lent_ptr fiber_executor<Return>::get_execution_context()
{
	return lend(m_execContext);
}
template<typename Return>
executor_context_const_lent_ptr fiber_executor<Return>::get_execution_context() const
{
	return lend(m_execContext);
}
template<typename Return>
bool fiber_executor<Return>::pending() const
{
	return m_state.get() == ExecutorState::Idle || m_state.get() == ExecutorState::Pending;
}

template<typename Return>
thread_executor<Return>::thread_executor(thread i_thread)
: m_execContext(std::move(i_thread))
, m_state(ExecutorState::Idle)
{
}
template<typename Return>
typename thread_executor<Return>::start_result thread_executor<Return>::execute(const sink_type& i_sink, const ddk::function<Return()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(StartErrorCode::StartNoCallable);
	}
	else
	{
		if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Executing))
		{
			m_execContext.start([=]()
			{
				try
				{
					if constexpr(std::is_same<void,Return>::value)
					{
						eval_unsafe(i_callable);

						while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

						if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
						{
							eval_unsafe(i_sink,_void);
						}
					}
					else
					{
						sink_reference res = eval_unsafe(i_callable);

						while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

						if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
						{
							eval_unsafe(i_sink,res);
						}
					}

					return;
				}
				catch(const async_exception& i_excp)
				{
					eval_unsafe(i_sink,i_excp);
				}
				catch(const std::exception& i_excp)
				{
					eval_unsafe(i_sink,async_exception{ i_excp.what() });
				}
				catch(...)
				{
					eval_unsafe(i_sink,async_exception{ "Unkwon exception" });
				}

				while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

				//since there has been an exception,if executing pass it to cancelled
				ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
			});

			return make_result<start_result>(ExecutorState::Executing);
		}
		else
		{
			return make_error<start_result>(executor_interface<Return()>::StartNotExecutable);
		}
	}
}
template<typename Return>
typename thread_executor<Return>::cancel_result thread_executor<Return>::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if (ddk::atomic_compare_exchange(m_state, ExecutorState::Idle, ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else if (ddk::atomic_compare_exchange(m_state, ExecutorState::Executing, ExecutorState::Cancelling))
	{
		if (i_cancelFunc != nullptr && eval_unsafe(i_cancelFunc))
		{
			m_state = ExecutorState::Cancelled;

			return ddk::success;
		}
		else
		{
			m_state = ExecutorState::Executing;

			std::this_thread::yield();

			return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
		}
	}
	else
	{
		return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
	}
}
template<typename Return>
executor_context_lent_ptr thread_executor<Return>::get_execution_context()
{
	return lend(m_execContext);
}
template<typename Return>
executor_context_const_lent_ptr thread_executor<Return>::get_execution_context() const
{
	return lend(m_execContext);
}
template<typename Return>
bool thread_executor<Return>::pending() const
{
	return m_state.get() == ExecutorState::Idle || m_state.get() == ExecutorState::Pending;
}

template<typename Return>
execution_context_executor<Return>::execution_context_executor(executor_context_lent_ptr i_execContext,unsigned char i_depth)
: m_execContext(i_execContext)
, m_state(ExecutorState::Idle)
, m_depth(i_depth)
{
}
template<typename Return>
typename execution_context_executor<Return>::start_result execution_context_executor<Return>::execute(const sink_type& i_sink,const ddk::function<Return()>& i_callable)
{
	if(i_callable == nullptr)
	{
		return make_error<start_result>(StartErrorCode::StartNoCallable);
	}
	else
	{
		executor_context_lent_ptr execContext = m_execContext;
		if(execContext == nullptr)
		{
			//by design, if no executor available, use deferred executor
			execContext = lend(s_execContext);
		}
		m_execContext = nullptr;

		auto callable = [=]()
		{
			try
			{
				if constexpr(std::is_same<void,Return>::value)
				{
					eval_unsafe(i_callable);

					while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

					if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
					{
						eval_unsafe(i_sink,_void);
					}
				}
				else
				{
					sink_reference res = eval_unsafe(i_callable);

					while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

					if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Executed))
					{
						eval_unsafe(i_sink,res);
					}
				}

				return;
			}
			catch(const async_exception& i_excp)
			{
				eval_unsafe(i_sink,i_excp);
			}
			catch(const std::exception& i_excp)
			{
				eval_unsafe(i_sink,async_exception{ i_excp.what() });
			}
			catch(...)
			{
				eval_unsafe(i_sink,async_exception{ "Unkwon exception" });
			}

			while(m_state.get() == ExecutorState::Cancelling) std::this_thread::yield();

			//since there has been an exception,if executing pass it to cancelled
			ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
		};

		if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Executing))
		{
			m_continuationToken = execContext->enqueue(callable,m_depth);

			if(!m_continuationToken)
			{
				ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Pending);
			}

			return make_result<start_result>(m_state.get());
		}
		else if(ddk::atomic_compare_exchange(m_state,ExecutorState::Pending,ExecutorState::Executing))
		{
			execContext->start(callable);

			return make_result<start_result>(m_state.get());
		}
		else
		{
			return make_error<start_result>(executor_interface<Return()>::StartNotExecutable);
		}
	}
}
template<typename Return>
typename execution_context_executor<Return>::cancel_result execution_context_executor<Return>::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Cancelled) ||
	   ddk::atomic_compare_exchange(m_state,ExecutorState::Pending,ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	else 
	{
		if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelling))
		{
			executor_context_lent_ptr execContext = m_execContext;
			if(execContext == nullptr)
			{
				//by design, if no executor available, use deferred executor
				execContext = lend(s_execContext);
			}

			if(execContext->dismiss(m_depth,std::move(m_continuationToken)) == false)
			{
				if(i_cancelFunc != nullptr && eval_unsafe(i_cancelFunc))
				{
					m_state = ExecutorState::Cancelled;

					return ddk::success;
				}
			}

			m_state = ExecutorState::Executing;

			std::this_thread::yield();

			return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
		}
		else
		{
			return make_error<cancel_result>(CancelErrorCode::CancelAlreadyExecuted);
		}
	}
}
template<typename Return>
executor_context_lent_ptr execution_context_executor<Return>::get_execution_context()
{
	return m_execContext;
}
template<typename Return>
executor_context_const_lent_ptr execution_context_executor<Return>::get_execution_context() const
{
	return m_execContext;
}
template<typename Return>
bool execution_context_executor<Return>::pending() const
{
	return m_state.get() == ExecutorState::Idle || m_state.get() == ExecutorState::Pending;
}

}
}
