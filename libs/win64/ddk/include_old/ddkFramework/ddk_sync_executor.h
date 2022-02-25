#pragma once

#include "ddk_async_executor_interface.h"
#include "ddk_executor_promise.h"
#include "ddk_sync_executor_impl.h"
#include "ddk_attachable.h"
#include "ddk_distribute_from_this.h"

namespace ddk
{

template<typename Return>
class async_executor : public async_cancellable_interface, public async_execute_interface<Return>, public distribute_from_this<async_executor<Return>>
{
	template<typename RReturn>
	friend distributed_reference_wrapper<async_executor<RReturn>> make_async_executor(const ddk::function<RReturn()>& i_function);
	friend class future<Return>;
	using typename async_execute_interface<Return>::StartErrorCode;
	typedef typename executor_interface<Return()>::start_result nested_start_result;
	typedef typename executor_interface<Return()>::sink_reference sink_reference;
	typedef typename executor_interface<Return()>::sink_result sink_result;

public:
	typedef distributed_reference_wrapper<async_executor<Return>> async_dist_ref;
	typedef distributed_reference_wrapper<const async_executor<Return>> async_const_dist_ref;
	using typename async_execute_interface<Return>::start_result;
	using typename async_cancellable_interface::cancel_result;
	typedef typename detail::private_async_state<Return>::reference reference;
	typedef typename detail::private_async_state<Return>::const_reference const_reference;
	typedef typename detail::private_async_state<Return>::rreference rreference;

	async_executor(const function<Return()>& i_function);
	async_executor(const async_executor&) = delete;
	async_executor(async_executor&& other);
	~async_executor();
	async_executor& operator=(const async_executor&) = delete;

	// immediat attach
	future<Return> attach(thread i_thread);
	future<Return> attach(fiber i_fiber);
	future<Return> attach(thread_sheaf i_threadSheaf);
	future<Return> attach(fiber_sheaf i_fiberSheaf);
	future<Return> attach(executor_context_lent_ptr i_asyncExecutorContext,unsigned char i_depth);
	future<Return> attach(cancellable_executor_unique_ref<Return> i_execImpl);
	future<Return> attach(attachable<Return> i_attachable);

	// delayed attach
	future<Return> deferred_attach(thread i_thread);
	future<Return> deferred_attach(fiber i_fiber);
	future<Return> deferred_attach(thread_sheaf i_threadSheaf);
	future<Return> deferred_attach(fiber_sheaf i_fiberSheaf);
	future<Return> deferred_attach(executor_context_lent_ptr i_asyncExecutorContext,unsigned char i_depth);
	future<Return> deferred_attach(cancellable_executor_unique_ref<Return> i_execImpl);
	future<Return> deferred_attach(attachable<Return> i_attachable);

	async_dist_ref store(promise<Return>& i_promise);
	async_dist_ref on_cancel(const ddk::function<bool()>& i_cancelFunc);
	future<Return> as_future();

protected:
	async_executor() = default;

private:
	bool notify() override;
	executor_context_lent_ptr get_execution_context() override;
	executor_context_const_lent_ptr get_execution_context() const override;
	start_result execute() override;
	cancel_result cancel() override;
	bool empty() const override;

	void set_result(sink_result i_value);

	cancellable_executor_unique_ptr<Return> m_executor;
	mutable executor_promise<Return> m_promise;
	ddk::function<Return()> m_function;
	ddk::function<bool()> m_cancelFunc;
};

template<>
class async_executor<void> : public async_executor<detail::void_t>
{
	template<typename Return>
	friend distributed_reference_wrapper<async_executor<Return>> make_async_executor(const ddk::function<Return()>& i_function);
	friend class future<void>;

public:
	typedef distributed_reference_wrapper<async_executor<detail::void_t>> async_dist_ref;
	typedef distributed_reference_wrapper<const async_executor<detail::void_t>> async_const_dist_ref;

    async_executor(const function<void()>& i_function)
    : async_executor<detail::void_t>([i_function]() -> detail::void_t { eval(i_function); return _void; })
    {}
};

}
