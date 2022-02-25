#pragma once

#include "ddk_stack_allocator_interface.h"
#include "ddk_execution_stack.h"

#ifndef DDK_STACK_ALLOCATOR_DEFAULT_NUM_GUARD_PAGES

#define DDK_STACK_ALLOCATOR_DEFAULT_NUM_GUARD_PAGES 2

#endif

namespace ddk
{
namespace detail
{

const size_t k_defaultGuardPages = DDK_STACK_ALLOCATOR_DEFAULT_NUM_GUARD_PAGES;

template<size_t NumGuardPages>
class dynamic_stack_allocator : public stack_allocator_interface
{
public:
	dynamic_stack_allocator() = default;

private:
	static const size_t s_numGuardPages = NumGuardPages;

	stack_alloc_const_dist_ref share() const override;
	stack_alloc_dist_ref share() override;
	void* reserve(size_t) const override;
	std::pair<void*,void*> allocate(void* i_ref, size_t i_size) const override;
	bool reallocate(detail::execution_stack& i_stackAddr, void* i_reason) const override;
	void deallocate(void*,size_t) const override;
	void release(void*,size_t) const override;
	size_t get_num_guard_pages() const override;
};

typedef dynamic_stack_allocator<k_defaultGuardPages> default_dynamic_stack_allocator;

}

stack_alloc_dist_ref make_default_dynamic_stack_allocator();

template<size_t NumGuardPages = detail::k_defaultGuardPages>
stack_alloc_dist_ref make_dynamic_stack_allocator();

}

#include "ddk_dynamic_stack_allocator.inl"
