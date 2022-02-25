#pragma once

#include "ddk_stack_allocator_interface.h"
#include "ddk_execution_stack.h"
#include "ddk_arena.h"

namespace ddk
{
namespace detail
{

template<size_t Size>
class static_stack_allocator : public stack_allocator_interface
{
public:
	static_stack_allocator() = default;
	~static_stack_allocator();

private:
	stack_alloc_const_dist_ref share() const override;
	stack_alloc_dist_ref share() override;
	void* reserve(size_t) const override;
	std::pair<void*,void*> allocate(void* i_ref, size_t i_size) const override;
	bool reallocate(execution_stack& i_stackAddr, void* i_reason) const override;
	void deallocate(void*,size_t) const override;
	void release(void*,size_t) const override;
	size_t get_num_guard_pages() const override;

	mutable arena<(Size+1) * 4096,1> m_arena;
	mutable bool m_underUse = false;
};

}

template<size_t Size>
stack_alloc_dist_ref make_static_stack_allocator();

}

#include "ddk_static_stack_allocator.inl"
