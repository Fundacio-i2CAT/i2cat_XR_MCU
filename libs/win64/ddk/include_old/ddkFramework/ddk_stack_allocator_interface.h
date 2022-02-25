#pragma once

#include <utility>
#include "ddk_shared_reference_wrapper.h"
#include "ddk_reference_wrapper.h"
#include "ddk_shared_from_this.h"

extern "C"
{
	void set_curr_thread_stack_limit(void*);
}

#if !defined(DDK_USE_CUSTOM_STACK_ALLOCATION_INFRASTRUCTURE) && defined(DDK_STACK_ALLOCATOR_NUM_COMMITED_PAGES) && defined(DDK_STACK_ALLOCATOR_DEFAULT_NUM_GUARD_PAGES)

#define DDK_STACK_ALLOCATOR_DEFAULT_NUM_RESERVED_PAGES DDK_STACK_ALLOCATOR_NUM_COMMITED_PAGES + DDK_STACK_ALLOCATOR_DEFAULT_NUM_GUARD_PAGES

#elif !defined(DDK_STACK_ALLOCATOR_DEFAULT_NUM_RESERVED_PAGES)

#define DDK_STACK_ALLOCATOR_DEFAULT_NUM_RESERVED_PAGES 60

#endif

namespace ddk
{
namespace detail
{

struct execution_stack;

}

class stack_allocator_interface : public distribute_from_this<stack_allocator_interface>
{
public:
	static const size_t k_maxNumStackPages = DDK_STACK_ALLOCATOR_DEFAULT_NUM_RESERVED_PAGES;

	virtual ~stack_allocator_interface() = default;
	virtual distributed_reference_wrapper<const stack_allocator_interface> share() const = 0;
	virtual distributed_reference_wrapper<stack_allocator_interface> share() = 0;
	virtual void* reserve(size_t) const = 0;
	virtual std::pair<void*,void*> allocate(void*,size_t) const = 0;
	virtual bool reallocate(detail::execution_stack&, void*) const = 0;
	virtual void deallocate(void*,size_t) const = 0;
	virtual void release(void*,size_t) const = 0;
	virtual size_t get_num_guard_pages() const = 0;

	static size_t s_pageSize;
};

typedef ddk::distributed_reference_wrapper<stack_allocator_interface> stack_alloc_dist_ref;
typedef ddk::distributed_reference_wrapper<const stack_allocator_interface> stack_alloc_const_dist_ref;
typedef ddk::distributed_pointer_wrapper<stack_allocator_interface> stack_alloc_dist_ptr;
typedef ddk::distributed_pointer_wrapper<const stack_allocator_interface> stack_alloc_const_dist_ptr;

typedef ddk::lent_reference_wrapper<stack_allocator_interface> stack_alloc_lent_ref;
typedef ddk::lent_reference_wrapper<const stack_allocator_interface> stack_alloc_const_lent_ref;
typedef ddk::lent_pointer_wrapper<stack_allocator_interface> stack_alloc_lent_ptr;
typedef ddk::lent_pointer_wrapper<const stack_allocator_interface> stack_alloc_const_lent_ptr;

template<typename Allocator, typename ... Args>
stack_alloc_dist_ref make_stack_allocator(Args&& ... i_args)
{
	return make_distributed_reference<Allocator>(std::forward<Args>(i_args) ...);
}

}


