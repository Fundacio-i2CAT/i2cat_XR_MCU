
namespace ddk
{
namespace detail
{

template<typename Return>
await_executor<Return>::await_executor()
: m_state(ExecutorState::Idle)
, m_callee(*this)
{
	m_callee.set_executor(this->ref_from_this());
}
template<typename Return>
await_executor<Return>::await_executor(const ddk::function<Return()>& i_callable)
: m_callable(i_callable)
, m_state(ExecutorState::Executing)
, m_callee(*this)
{
	m_callee.set_executor(this->ref_from_this());

	m_callee.start_from(m_caller,m_callable);
}
template<typename Return>
await_executor<Return>::await_executor(const ddk::function<Return()>& i_callable, stack_allocator i_stackAlloc)
: m_callable(i_callable)
, m_callee(std::move(i_stackAlloc),*this)
, m_state(ExecutorState::Executing)
{
	m_callee.set_executor(this->ref_from_this());

	m_callee.start_from(m_caller,m_callable);
}
template<typename Return>
await_executor<Return>::await_executor(const await_executor& other)
: await_executor(other,other.get_stack_allocator())
{
}
template<typename Return>
await_executor<Return>::await_executor(const await_executor& other,const stack_allocator& i_stackAllocator)
: await_executor(other.m_callable,{ i_stackAllocator.get_alloc_impl(), i_stackAllocator.get_num_max_pages() })
{
}
template<typename Return>
await_executor<Return>::~await_executor()
{
	if(ddk::atomic_compare_exchange(m_state,ExecutorState::Executing,ExecutorState::Cancelling))
	{
		m_callee.stop();

		m_state = ExecutorState::Cancelled;
	}
}
template<typename Return>
await_executor<Return>& await_executor<Return>::operator=(const await_executor& other)
{
	m_callee.stop();

	if(other.m_callable != nullptr)
	{
		m_state = ExecutorState::Executing;
		m_callable = other.m_callable;
		m_callee.start_from(m_caller,m_callable);
	}
	else
	{
		m_state = ExecutorState::Idle;
	}

	return *this;
}
template<typename Return>
bool await_executor<Return>::resume()
{
	if(m_callee.resume_from(m_caller) == FiberExecutionState::Done)
	{
		m_state = ExecutorState::Executed;

		return false;
	}
	else
	{
		return true;
	}
}
template<typename Return>
bool await_executor<Return>::resume(const sink_type& i_sink)
{
	if(m_callee.resume_from(m_caller) == FiberExecutionState::Done)
	{
		m_state = ExecutorState::Executed;

		return false;
	}
	else
	{
		if constexpr(std::is_same<Return,detail::void_t>::value)
		{
			eval(i_sink,_void);
		}
		else if constexpr(std::is_same<Return,void>::value)
		{
			eval(i_sink);
		}
		else
		{
			if(typed_yielder_context<Return>* newContext = m_caller.get_typed_context<Return>())
			{
				eval(i_sink,newContext->get_value());
			}
			else
			{
				throw async_exception{ "No yielded value when retorning from secondary context" };
			}
		}

		return true;
	}
}
template<typename Return>
const stack_allocator& await_executor<Return>::get_stack_allocator() const
{
	return m_callee.get_stack_allocator();
}
template<typename Return>
void await_executor<Return>::yield()
{
	m_callee.resume_to(m_caller,nullptr);
}
template<typename Return>
ExecutorState await_executor<Return>::get_state() const
{
	return (m_state.get() == ExecutorState::Idle) ? ExecutorState::Idle : ExecutorState::Executed;
}
template<typename Return>
void await_executor<Return>::yield(yielder_context* i_context)
{
	if(typed_yielder_context<Return>* newContext = static_cast<typed_yielder_context<Return>*>(i_context))
	{
		m_callee.resume_to(m_caller,i_context);
	}
}
template<typename Return>
void await_executor<Return>::suspend(yielder_context*)
{
	const ExecutorState prevState = ddk::atomic_compare_exchange_val(m_state,ExecutorState::Executing,ExecutorState::Cancelling);

	if(prevState == ExecutorState::Executing || prevState == ExecutorState::Cancelling)
	{
		throw suspend_exception(m_callee.get_id());
	}
}
template<typename Return>
bool await_executor<Return>::activate(fiber_id i_id,const ddk::function<void()>& i_callable)
{
	return true;
}
template<typename Return>
bool await_executor<Return>::deactivate(fiber_id i_id)
{
	if(m_callee.get_id() == i_id)
	{
		m_callee.resume_from(m_caller);

		return true;
	}
	else
	{
		DDK_FAIL("Trying to deactivate fiber from the wrong executor");

		return false;
	}
}
template<typename Return>
void await_executor<Return>::unregister(fiber_id)
{
}

}
}
