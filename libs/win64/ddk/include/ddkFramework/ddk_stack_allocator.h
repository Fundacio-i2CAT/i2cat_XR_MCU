#pragma once

#include <cstddef>
#include <utility>
#include "ddk_thread_local.h"
#include <unordered_map>
#include "ddk_macros.h"
#include "ddk_stack_allocator_interface.h"
#include "ddk_fiber_defs.h"

#ifndef DDK_STACK_ALLOCATOR_NUM_COMMITED_PAGES

#define DDK_STACK_ALLOCATOR_NUM_COMMITED_PAGES 1

#endif


#if defined(WIN32)

#include <windows.h>

#elif defined(__LINUX__) || defined(__APPLE__)

#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>

#endif

namespace ddk
{

class stack_allocator
{
#if defined(WIN32)

	friend LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo);

#elif defined(__LINUX__) or defined(__APPLE__)

	friend void segfault_sigaction(int i_code,siginfo_t* i_sigInfo,void* i_context);

#endif

	friend inline stack_allocator share(const stack_allocator& other)
	{
		return { other.m_stackAllocImpl->share(),other.m_numMaxPages };
	}

	static const size_t s_num_ready_to_use_pages = DDK_STACK_ALLOCATOR_NUM_COMMITED_PAGES;

public:
    typedef stack_allocator allocator;
    typedef void type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef std::ptrdiff_t difference_type;

	stack_allocator(stack_alloc_const_dist_ref i_stackAllocImpl, size_t i_numMaxPages = stack_allocator_interface::k_maxNumStackPages);
	stack_allocator(const stack_allocator&) = delete;
	stack_allocator(stack_allocator&& other);
	~stack_allocator();
    detail::execution_stack allocate() const;
    void deallocate(const detail::execution_stack&) const;
	size_t get_num_max_pages() const;
	size_t get_num_guard_pages() const;
	stack_alloc_const_dist_ref get_alloc_impl() const;
	stack_alloc_const_lent_ref get_alloc_impl_ref() const;

	stack_allocator& operator=(const stack_allocator&) = delete;
	stack_allocator& operator=(stack_allocator&&) = delete;

private:
	stack_alloc_const_dist_ref m_stackAllocImpl;
	const size_t m_numMaxPages;
};

}
