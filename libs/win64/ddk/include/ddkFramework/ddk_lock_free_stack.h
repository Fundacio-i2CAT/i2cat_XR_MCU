#pragma once

#include "ddk_lock_free_stack_node.h"
#include "ddk_optional.h"
#include "ddk_global_allocators.h"
#include "ddk_system_allocator.h"
#include "ddk_spin_lock.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"
#include <thread>

namespace ddk
{

const size_t k_maxNumberOfPivotChangeRetries = 10;

template<typename T, typename Allocator = typed_system_allocator<lock_free_stack_node<T>>>
struct single_consumer_lock_free_stack
{
public:
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(Allocator,Args...))
	single_consumer_lock_free_stack(Args&& ... i_args);
	~single_consumer_lock_free_stack();
	template<typename ... Args>
	void push(Args&& ... i_args);
	optional<T> pop();
	inline void clear();
	inline bool empty() const;

protected:
	void _push(lock_free_stack_node<T>* i_newNode);
	template<typename ... Args>
	lock_free_stack_node<T>* allocate_node(Args&& ... i_args);
	void deallocate_node(lock_free_stack_node<T>* i_ptr);

	mutable lock_free_stack_node<T> m_head;
	mutable lock_free_stack_node<T> m_tail;
	Allocator m_allocator;
};

template<typename T,typename Allocator = typed_system_allocator<lock_free_stack_node<T>>>
struct multiple_consumer_lock_free_stack : single_consumer_lock_free_stack<T,Allocator>
{
public:
	using single_consumer_lock_free_stack<T,Allocator>::single_consumer_lock_free_stack;

	optional<T> pop();

private:
	exclusive_spin_lock m_barrier;
};

}

#include "ddk_lock_free_stack.inl"