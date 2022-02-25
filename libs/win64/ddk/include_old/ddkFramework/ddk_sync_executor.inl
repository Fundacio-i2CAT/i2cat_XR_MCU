#include "ddk_executor_promise.inl"

namespace ddk
{

template<typename Return>
distributed_reference_wrapper<async_executor<Return>> make_async_executor(const function<Return()>& i_function)
{
	static const fixed_size_allocator* s_allocator = get_fixed_size_allocator(size_of_distributed_allocation<async_executor<Return>>());

	return (s_allocator) ? make_distributed_reference<async_executor<Return>>(*s_allocator,i_function)
		: make_distributed_reference<async_executor<Return>>(i_function);
}

template<typename Return>
async_executor<Return>::async_executor(const function<Return()>& i_function)
: m_function(i_function)
, m_executor(make_executor<detail::deferred_executor<Return>>())
{
}
template<typename Return>
async_executor<Return>::async_executor(async_executor&& other)
: m_function(std::move(other.m_function))
, m_executor(std::move(other.m_executor))
, m_promise(std::move(other.m_promise))
{
}
template<typename Return>
async_executor<Return>::~async_executor()
{
	if(m_executor && m_executor->pending())
	{
		eval(m_function);
	}
}
template<typename Return>
future<Return> async_executor<Return>::attach(thread i_thread)
{
	m_executor = make_executor<detail::thread_executor<Return>>(std::move(i_thread));

	start_result execRes = execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::attach(fiber i_fiber)
{
	m_executor = make_executor<detail::fiber_executor<Return>>(std::move(i_fiber));

	start_result execRes = execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::attach(thread_sheaf i_threadSheaf)
{
	//at some point put a composed callable here
	async_dist_ref newAsyncExecutor = make_distributed_reference<async_executor<detail::void_t>>(make_function([capturedFunction = m_function]() { eval(capturedFunction); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::thread_sheaf_executor>(std::move(i_threadSheaf));

	m_promise.detach();
	m_executor.clear();

	start_result execRes = newAsyncExecutor->execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return newAsyncExecutor->as_future();
}
template<typename Return>
future<Return> async_executor<Return>::attach(fiber_sheaf i_fiberSheaf)
{
	async_dist_ref newAsyncExecutor = make_distributed_reference<async_executor<detail::void_t>>(make_function([capturedFunction = m_function]() { eval(capturedFunction); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::fiber_sheaf_executor>(std::move(i_fiberSheaf));

	m_promise.detach();
	m_executor.clear();

	start_result execRes = newAsyncExecutor->execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return newAsyncExecutor->as_future();
}
template<typename Return>
future<Return> async_executor<Return>::attach(executor_context_lent_ptr i_asyncExecutorContext,unsigned char i_depth)
{
	m_executor = make_executor<detail::execution_context_executor<Return>>(i_asyncExecutorContext,i_depth);

	start_result execRes = execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::attach(cancellable_executor_unique_ref<Return> i_execImpl)
{
	m_executor = std::move(i_execImpl);

	start_result execRes = execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::attach(attachable<Return> i_attachable)
{
	m_executor = std::move(i_attachable.m_executorImpl);

	start_result execRes = execute();

	DDK_ASSERT(execRes != StartErrorCode::AlreadyDone,"Trying to execute an alerady executed async executor");

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(thread i_thread)
{
	m_executor = make_executor<detail::thread_executor<Return>>(std::move(i_thread));

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(fiber i_fiber)
{
	m_executor = make_executor<detail::fiber_executor<Return>>(std::move(i_fiber));

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(thread_sheaf i_threadSheaf)
{
	//at some point put a composed callable here
	async_dist_ref newAsyncExecutor = make_distributed_reference<async_executor<detail::void_t>>(make_function([capturedFunction = m_function]() { eval(capturedFunction); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::thread_sheaf_executor>(std::move(i_threadSheaf));

	m_promise.detach();
	m_executor.clear();

	return newAsyncExecutor->as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(fiber_sheaf i_fiberSheaf)
{
	async_dist_ref newAsyncExecutor = make_distributed_reference<async_executor<detail::void_t>>(make_function([capturedFunction = m_function]() { eval(capturedFunction); return _void; }));

	newAsyncExecutor->m_executor = make_executor<detail::fiber_sheaf_executor>(std::move(i_fiberSheaf));

	m_promise.detach();
	m_executor.clear();

	return newAsyncExecutor->as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(executor_context_lent_ptr i_asyncExecutorContext,unsigned char i_depth)
{
	m_executor = make_executor<detail::execution_context_executor<Return>>(i_asyncExecutorContext,i_depth);

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(cancellable_executor_unique_ref<Return> i_execImpl)
{
	m_executor = std::move(i_execImpl);

	return as_future();
}
template<typename Return>
future<Return> async_executor<Return>::deferred_attach(attachable<Return> i_attachable)
{
	m_executor = std::move(i_attachable.m_executorImpl);

	return as_future();
}
template<typename Return>
typename async_executor<Return>::async_dist_ref async_executor<Return>::store(promise<Return>& i_promise)
{
	m_promise = i_promise;

	return this->ref_from_this();
}
template<typename Return>
typename async_executor<Return>::async_dist_ref async_executor<Return>::on_cancel(const ddk::function<bool()>& i_cancelFunc)
{
	m_cancelFunc = i_cancelFunc;

	return this->ref_from_this();
}
template<typename Return>
typename async_executor<Return>::start_result async_executor<Return>::execute()
{
    if(m_executor == nullptr)
    {
        throw async_exception{"Trying to execute empty executor"};
    }

	const nested_start_result execRes = m_executor->execute(make_function(this,&async_executor<Return>::set_result),
	make_function([self = this->ref_from_this()]() mutable
	{
		return eval(self->m_function);
	}));

	if(execRes == success)
	{
		const ExecutorState currState = execRes.get();

		if(currState != ExecutorState::Idle)
		{
			return success;
		}
		else
		{
			return make_error<start_result>(StartErrorCode::NotAvailable);
		}
	}
	else
	{
		return make_error<start_result>(StartErrorCode::NotAvailable);
	}
}
template<typename Return>
void async_executor<Return>::set_result(sink_result i_value)
{
	if(i_value.template is<sink_reference>())
	{
		m_promise.set_value(std::forward<sink_reference>(i_value.template get<sink_reference>()));
	}
	else
	{
		m_promise.set_exception(i_value.template get<async_exception>());
	}
}
template<typename Return>
future<Return> async_executor<Return>::as_future()
{
	m_promise.attach(this->ref_from_this());

	return m_promise.get_future();
}
template<typename Return>
typename async_executor<Return>::cancel_result async_executor<Return>::cancel()
{
    if(m_executor == nullptr)
    {
        return make_error<cancel_result>(CancelNoAsync);
    }
	else
	{
		const cancel_result cancelRes = m_executor->cancel(m_cancelFunc);

		if (cancelRes == success)
		{
			m_promise.set_exception(async_exception{ "task has been cancelled.", AsyncExceptionCode::Cancel });

			m_executor = nullptr;
		}

		return cancelRes;
	}
}
template<typename Return>
bool async_executor<Return>::empty() const
{
	return m_executor == nullptr;
}
template<typename Return>
bool async_executor<Return>::notify()
{
	//if not executed, excute
	if(m_executor && m_executor->pending())
	{
		start_result execRes = execute();

		DDK_ASSERT(execRes == success,"Error while trying to execute async executor");

		return true;
	}
	else
	{
		return false;
	}
}
template<typename Return>
executor_context_lent_ptr async_executor<Return>::get_execution_context()
{
	return (m_executor != nullptr) ? m_executor->get_execution_context() : nullptr;
}
template<typename Return>
executor_context_const_lent_ptr async_executor<Return>::get_execution_context() const
{
	return (m_executor != nullptr) ? m_executor->get_execution_context() : nullptr;
}

}
