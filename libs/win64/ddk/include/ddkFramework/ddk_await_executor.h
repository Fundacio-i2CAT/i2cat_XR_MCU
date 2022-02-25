#pragma once

#include "ddk_function.h"
#include "ddk_fiber_scheduler_interface.h"
#include "ddk_stack_allocator.h"
#include "ddk_atomics.h"
#include "ddk_fiber_impl.h"
#include "ddk_executor_interface.h"

namespace ddk
{
namespace detail
{

template<typename Return>
class await_executor: public scheduler_interface, private yielder_interface, protected lend_from_this<await_executor<Return>,detail::scheduler_interface>
{
    friend class lend_from_this<await_executor<Return>,detail::scheduler_interface>;

public:
	typedef typename detail::sink_type_resolver<Return>::result_t sink_result;
	typedef typename detail::sink_type_resolver<Return>::type sink_type;

	await_executor();
	await_executor(const ddk::function<Return()>& i_callable);
	await_executor(const ddk::function<Return()>& i_callable, stack_allocator i_stackAlloc);
	await_executor(const await_executor& other);
	~await_executor();
	await_executor& operator=(const await_executor&);

	inline bool resume();
	inline bool resume(const sink_type& i_sink);
	inline const stack_allocator& get_stack_allocator() const;
	inline void yield();
	ExecutorState get_state() const;

private:
	await_executor(const await_executor& other,const stack_allocator&);
	void yield(yielder_context* i_context) override;
	void suspend(yielder_context* = nullptr) override;

	bool activate(fiber_id,const ddk::function<void()>&) override;
	bool deactivate(fiber_id) override;
	void unregister(fiber_id) override;

	ddk::function<Return()> m_callable;
	this_fiber_t m_caller;
	fiber_impl m_callee;
	atomic<ExecutorState::underlying_type> m_state;
};

}
}

#include "ddk_await_executor.inl"
