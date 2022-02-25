#pragma once

#include "ddk_fiber_defs.h"
#include "ddk_fiber_yielder.h"
#include "ddk_execution_stack.h"
#include "ddk_ucontext.h"

#ifdef DDK_USE_CUSTOM_STACK_ALLOCATION_INFRASTRUCTURE \

#define load_switch_execution_context(i_oldCtxt,i_newCtxt) \
	\
	set_current_execution_context(i_newCtxt); \
	\
	load_switch_execution_stack(i_oldCtxt.m_stack,i_newCtxt.m_stack) \
	\
	ddk::swap_context(&i_oldCtxt.m_context,&i_newCtxt.m_context);

#define switch_execution_context(i_oldCtxt,i_newCtxt) \
	\
	set_current_execution_context(i_newCtxt); \
	\
	switch_execution_stack(i_newCtxt.m_stack) \
	\
	ddk::swap_context(&i_oldCtxt.m_context,&i_newCtxt.m_context);

#else

#define load_switch_execution_context(i_oldCtxt,i_newCtxt) \
	\
	set_current_execution_context(i_newCtxt); \
	\
	get_curr_thread_stack(&(i_oldCtxt.m_stack)); \
	\
	load_switch_execution_stack(i_oldCtxt.m_stack,i_newCtxt.m_stack) \
	\
	ddk::swap_context(&i_oldCtxt.m_context,&i_newCtxt.m_context);

#define switch_execution_context(i_oldCtxt,i_newCtxt) \
	\
	set_current_execution_context(i_newCtxt); \
	\
	get_curr_thread_stack(&(i_oldCtxt.m_stack)); \
	\
	switch_execution_stack(i_newCtxt.m_stack) \
	\
	ddk::swap_context(&i_oldCtxt.m_context,&i_newCtxt.m_context);

#endif

#define switch_execution(i_newCtxt) \
	\
	switch_execution_stack(i_newCtxt.m_stack) \
	\
	set_current_execution_context(i_newCtxt);

namespace ddk
{
namespace detail
{

struct execution_context
{
	template<typename ... Args>
	friend inline void make_execution_context(execution_context& i_newCtxt, execution_context& i_oldCtxt, Args&& ... i_args)
	{
		ddk::make_context(&i_newCtxt.m_context,&i_oldCtxt.m_context,std::forward<Args>(i_args) ...);
	}

public:
	execution_context(fiber_id i_id);
	execution_context(fiber_id i_id,yielder_interface& i_yielder);
	execution_context(fiber_id i_id,stack_alloc_const_lent_ref i_allocator);
	execution_context(fiber_id i_id,yielder_interface& i_yielder,stack_alloc_const_lent_ref i_allocator);

	inline fiber_id get_id() const
	{
		return m_id;
	}
	void set_yielder(yielder_interface* i_yielder);
	inline yielder_interface* get_yielder()
	{
		return m_yielder;
	}
	inline const yielder_interface* get_yielder() const
	{
		return m_yielder;
	}
	void attach_stack(const execution_stack& i_stack);
	inline execution_stack& get_stack()
	{
		return m_stack;
	}
	inline const execution_stack& get_stack() const
	{
		return m_stack;
	}
	void start();
	void stop();
	inline bool is_stopped() const
	{
		return m_stopped;
	}
	template<typename T>
	inline T* get_typed_context()
	{
		return reinterpret_cast<T*>(m_context.uc_link);
	}
	template<typename T>
	inline void set_typed_context(T* i_context)
	{
		m_context.uc_link = reinterpret_cast<ucontext_t*>(i_context);
	}

//private:
	const fiber_id m_id;
	yielder_interface* m_yielder;
	execution_stack m_stack;
	mutable ucontext_t m_context;
	bool m_stopped = false;
};

}
}
