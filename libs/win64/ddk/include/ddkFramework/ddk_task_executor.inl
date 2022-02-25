
#include "ddk_lock_guard.h"
#include "ddk_async_exceptions.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename Return>
task_executor::pending_task_impl<Return>::pending_task_impl(const function<Return()>& i_task)
: m_executor(make_async_executor(i_task))
{
}
template<typename Return>
task_executor::pending_task_impl<Return>::~pending_task_impl()
{
}
template<typename Return>
void task_executor::pending_task_impl<Return>::execute(thread i_thread)
{
	if(async_cancellable_dist_ptr _executor = std::move(m_executor))
	{
		if(executor_context_lent_ptr _executorImpl = _executor->get_execution_context())
		{
			static_lent_cast<delayed_task_execution_context>(_executorImpl)->attach(std::move(i_thread));
		}
		else
		{
			throw async_exception{ "Trying to execute task with no context." };
		}
	}
	else
	{
		throw async_exception{"Trying to execute empty task."};
	}
}
template<typename Return>
bool task_executor::pending_task_impl<Return>::execute()
{
	if(async_execute_dist_ptr<Return> _executor = std::move(m_executor))
	{
		return static_cast<bool>(_executor->execute());
	}
	else
	{
		return false;
	}
}
template<typename Return>
bool task_executor::pending_task_impl<Return>::cancel()
{
	if(async_cancellable_dist_ptr _executor = std::move(m_executor))
	{
		if(_executor->cancel())
		{
			return true;
		}
	}

	return false;
}
template<typename Return>
bool task_executor::pending_task_impl<Return>::empty()
{
	if(async_execute_dist_ptr<Return> _executor = m_executor)
	{
		return _executor->empty();
	}
	else
	{
		return true;
	}
}
template<typename Return>
future<Return> task_executor::pending_task_impl<Return>::as_future()
{
	static const fixed_size_allocator* s_allocator = get_fixed_size_allocator(size_of_unique_allocation<detail::delayed_task_executor<Return>>());

	return m_executor->attach((s_allocator) ? make_unique_reference<detail::delayed_task_executor<Return>>(*s_allocator)
											: make_unique_reference<detail::delayed_task_executor<Return>>());
}

template<typename Return>
future<Return> task_executor::enqueue(const function<Return()>& i_task)
{
	if(m_state == Running)
	{
		static const fixed_size_allocator* s_allocator = get_fixed_size_allocator(size_of_unique_allocation<pending_task_impl<Return>>());

		unique_pending_impl_task<Return> newTask = (s_allocator) ? make_unique_reference<pending_task_impl<Return>>(*s_allocator,i_task)
																: make_unique_reference<pending_task_impl<Return>>(i_task);

		future<Return> taskFuture = newTask->as_future();

		if(m_numPendingTasks.get() > m_maxNumPendingTasks)
		{
			if(optional<unique_pending_task> optTask = m_pendingTasks.pop())
			{
				unique_pending_task newTask = optTask.extract();

				newTask->cancel();

				atomic_post_decrement(m_numPendingTasks);
			}
		}

		m_pendingTasks.push(std::move(newTask));

		atomic_post_increment(m_numPendingTasks);

		m_updateThread.signal_thread();

		return std::move(taskFuture);
	}
	else
	{
		throw future_exception{"Trying to enqueue tasks over a stopped task executor."};
	}
}

}