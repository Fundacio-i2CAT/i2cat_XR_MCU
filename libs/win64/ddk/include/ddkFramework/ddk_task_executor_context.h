#pragma once

#include "ddk_executor_interface.h"
#include "ddk_sync_executor_context.h"
#include <queue>

namespace ddk
{

class delayed_task_execution_context : public executor_context_interface,public lend_from_this<delayed_task_execution_context>
{
public:
	void start(const function<void()>&);
	void attach(thread i_thread);
	bool cancel();

private:
	continuation_token enqueue(const function<void()>&, unsigned char i_depth) override;
	bool dismiss(unsigned char i_depth,continuation_token i_token) override;
	void clear() override;

	mutex m_mutex;
	function<void()> m_function;
	thread m_thread;
	detail::async_executor_recipients m_recipients;
};

namespace detail
{

template<typename Return>
class delayed_task_executor : public cancellable_executor_interface<Return()>
{
public:
	delayed_task_executor();

	lent_reference_wrapper<delayed_task_execution_context> get_context();

private:
	typedef typename executor_interface<Return()>::sink_reference sink_reference;
	typedef typename executor_interface<Return()>::sink_type sink_type;
	using typename cancellable_executor_interface<Return()>::start_result;
	using typename cancellable_executor_interface<Return()>::StartErrorCode;
	using typename cancellable_executor_interface<Return()>::cancel_result;
	using typename cancellable_executor_interface<Return()>::CancelErrorCode;

	start_result execute(const sink_type& i_sink,const ddk::function<Return()>& i_callable) override;
	cancel_result cancel(const ddk::function<bool()>& i_cancelFunc) override;
	executor_context_lent_ptr get_execution_context() override;
	executor_context_const_lent_ptr get_execution_context() const override;
	bool pending() const override;

	delayed_task_execution_context m_execContext;
	atomic<ExecutorState::underlying_type> m_state;
};

}
}

#include "ddk_task_executor_context.inl"
