#pragma once

#include "ddk_fiber_defs.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_fiber_local.h"
#include "ddk_stack_allocator.h"
#include "ddk_thread_yielder.h"
#include "ddk_fiber_scheduler_interface.h"
#include "ddk_execution_context.h"

namespace ddk
{
namespace detail
{

struct this_fiber_t
{
public:
	this_fiber_t() = default;

	fiber_id get_id() const;
	void attach_context();
	void detach_context();
	execution_context& get_execution_context();
	const execution_context& get_execution_context() const;
	yielder_context* get_context() const;
	template<typename T>
	typed_yielder_context<T>* get_typed_context() const;
    void set_typed_context(yielder_context* i_context);

private:
	execution_context* m_execContext = nullptr;
};

struct fiber_impl
{
public:
	fiber_impl(yielder_interface& i_yielder);
	fiber_impl(stack_allocator i_stackAlloc, yielder_interface& i_yielder);
	fiber_impl(const fiber_impl&) = delete;
	fiber_impl(fiber_impl&& other);
	~fiber_impl();
	fiber_impl& operator=(const fiber_impl&) = delete;
	template<typename Return>
	void start_from(this_fiber_t& other, const ddk::function<Return()>& i_function);
	void start(const ddk::function<void()>& i_function);
	void stop();
	FiberExecutionState resume_from(this_fiber_t& other);
	void resume_to(this_fiber_t& other, yielder_context* i_context = nullptr);
	fiber_id get_id() const;
	void set_state(FiberExecutionState i_state);
	inline FiberExecutionState get_state() const
	{
		return m_state;
	}
	const stack_allocator& get_stack_allocator() const;
	void set_executor(scheduler_interface_lent_ptr i_executor);
	bool joinable() const;
	bool operator==(const fiber_impl& other) const;

private:
	fiber_id m_id;
	scheduler_interface_lent_ptr m_executor = nullptr;
	FiberExecutionState m_state;
	stack_allocator m_alloc;
	mutable execution_context m_fiberContext;
};

typedef unique_reference_wrapper<fiber_impl> fiber_impl_unique_ref;
typedef unique_reference_wrapper<const fiber_impl> fiber_impl_const_unique_ref;
typedef unique_pointer_wrapper<fiber_impl> fiber_impl_unique_ptr;
typedef unique_pointer_wrapper<const fiber_impl> fiber_impl_const_unique_ptr;

typedef lent_reference_wrapper<fiber_impl> fiber_impl_lent_ref;
typedef lent_reference_wrapper<const fiber_impl> fiber_impl_const_lent_ref;
typedef lent_pointer_wrapper<fiber_impl> fiber_impl_lent_ptr;
typedef lent_pointer_wrapper<const fiber_impl> fiber_impl_const_lent_ptr;

}

}

#include "ddk_fiber_impl.inl"
