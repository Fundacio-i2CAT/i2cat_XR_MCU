
namespace ddk
{
namespace detail
{

template<typename Return>
delayed_task_executor<Return>::delayed_task_executor()
: m_state(ExecutorState::Idle)
{
}
template<typename Return>
lent_reference_wrapper<delayed_task_execution_context> delayed_task_executor<Return>::get_context()
{
	return lend(m_execContext);
}
template<typename Return>
typename delayed_task_executor<Return>::start_result delayed_task_executor<Return>::execute(const sink_type& i_sink,const ddk::function<Return()>& i_callable)
{
	if(i_callable != nullptr)
	{
		if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Executing))
		{
			try
			{
				m_execContext.start([i_sink,i_callable]()
				{
					eval_unsafe(i_sink,eval_unsafe(i_callable));
				});
			}
			catch(...)
			{
				ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelled);
			}
		}

		return make_result<start_result>(m_state.get());
	}
	else
	{
		return make_result<start_result>(ExecutorState::Idle);
	}
}
template<typename Return>
typename delayed_task_executor<Return>::cancel_result delayed_task_executor<Return>::cancel(const ddk::function<bool()>& i_cancelFunc)
{
	if(ddk::atomic_compare_exchange(m_state,ExecutorState::Idle,ExecutorState::Cancelled))
	{
		return ddk::success;
	}
	if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelling))
	{
		if(m_execContext.cancel())
		{
			m_state = ExecutorState::Cancelled;

			return ddk::success;
		}
		else if(i_cancelFunc != nullptr && eval_unsafe(i_cancelFunc))
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
executor_context_lent_ptr delayed_task_executor<Return>::get_execution_context()
{
	return lend(m_execContext);
}
template<typename Return>
executor_context_const_lent_ptr delayed_task_executor<Return>::get_execution_context() const
{
	return lend(m_execContext);
}
template<typename Return>
bool delayed_task_executor<Return>::pending() const
{
	return m_state.get() == ExecutorState::Idle || m_state.get() == ExecutorState::Pending;
}

}
}