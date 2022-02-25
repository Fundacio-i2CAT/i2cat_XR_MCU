#pragma once

#include "ddk_stack_allocator_interface.h"

extern "C"
{
	void get_curr_thread_stack(void*);
	void set_curr_thread_stack(void*);
}

#if defined(DDK_USE_CUSTOM_STACK_ALLOCATION_INFRASTRUCTURE)

#define load_switch_execution_stack(i_oldStack,i_newStack) \
	\
	get_curr_thread_stack(&(i_oldStack)); \
	\
	set_curr_thread_stack(&(i_newStack));

#else

#define load_switch_execution_stack(i_oldStack,i_newStack) \
	\
	set_curr_thread_stack(&(i_newStack));

#endif

#define switch_execution_stack(i_newStack) \
	\
	set_curr_thread_stack(&(i_newStack));

namespace ddk
{
namespace detail
{

struct execution_stack
{

public:
	execution_stack() = default;
	execution_stack(stack_alloc_const_lent_ref i_allocator);
	execution_stack(void* i_init,void* i_end,void* i_dealloc);

	void attach(void* i_init, void* i_end, void* i_dealloc);

	void set_end(void* i_end);
	void set_dealloc(void* i_dealloc);
	inline void* get_init() const
	{
		return m_initStack;
	}
	inline void* get_end() const
	{
		return m_endStack;
	}
	inline void* get_dealloc() const
	{
		return m_deallocStack;
	}
	inline size_t get_size() const
	{
		return reinterpret_cast<char*>(m_initStack) - reinterpret_cast<char*>(m_endStack);
	}
	inline size_t get_dealloc_size() const
	{
		return reinterpret_cast<char*>(m_initStack) - reinterpret_cast<char*>(m_deallocStack);
	}
	inline stack_alloc_const_lent_ptr& get_allocator()
	{
		return m_stackAllocImpl;
	}

private:
	void* m_initStack = nullptr;
	void* m_endStack = nullptr;
	void* m_deallocStack = nullptr;
	stack_alloc_const_lent_ptr m_stackAllocImpl = nullptr;
};

}
}