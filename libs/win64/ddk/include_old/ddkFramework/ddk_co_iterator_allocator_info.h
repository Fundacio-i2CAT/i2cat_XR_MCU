#pragma once

#include "ddk_dynamic_stack_allocator.h"
#include "ddk_static_stack_allocator.h"
#include <vector>

namespace ddk
{

template<typename>
struct co_iterator_allocator_info;

template<typename Iterable>
struct co_iterator_allocator_info
{
	static const size_t s_max_num_pages = stack_allocator_interface::k_maxNumStackPages;
	typedef detail::default_dynamic_stack_allocator allocator;
};

template<typename T, typename Alloc>
struct co_iterator_allocator_info<std::vector<T,Alloc>>
{
	//although max num pages coul be 1, we shall reserve 3 pages because of stack unwinding
	static const size_t s_max_num_pages = 6;

	typedef detail::static_stack_allocator<s_max_num_pages> allocator;
};

//...

}