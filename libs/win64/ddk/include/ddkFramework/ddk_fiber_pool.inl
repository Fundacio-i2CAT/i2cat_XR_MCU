
#include "ddk_pool_stack_allocator.h"
#include "ddk_dynamic_stack_allocator.h"

namespace ddk
{

template<typename Comparator>
fiber_pool::fiber_pool(Policy i_policy, size_t i_initialSize, fiber_scheduler_dist_ref<Comparator> i_fiberScheduler, size_t i_maxNumPagesPerFiber)
: m_fiberScheduler(i_fiberScheduler)
, m_policy(i_policy)
, m_stackAllocator(make_shared_reference<detail::pool_stack_allocator>(make_shared_reference<detail::default_dynamic_stack_allocator>(),i_initialSize,i_maxNumPagesPerFiber))
{
	m_fiberCtr.reserve(i_initialSize);
	for(size_t fiberIndex=0;fiberIndex<i_initialSize;++fiberIndex)
	{
		m_fiberCtr.emplace_back(new detail::fiber_impl(m_stackAllocator,*m_fiberScheduler));
	}

	m_fiberScheduler->start();
}
template<typename Comparator>
fiber_pool::fiber_pool(Policy i_policy, size_t i_initialSize, fiber_scheduler_dist_ref<Comparator> i_fiberScheduler, stack_alloc_const_dist_ref i_nestedAlloc, size_t i_maxNumPagesPerFiber)
: m_fiberScheduler(i_fiberScheduler)
, m_policy(i_policy)
, m_stackAllocator(make_shared_reference<detail::pool_stack_allocator>(i_nestedAlloc,i_initialSize,i_maxNumPagesPerFiber))
{
	m_fiberCtr.reserve(i_initialSize);
	for(size_t fiberIndex=0;fiberIndex<i_initialSize;++fiberIndex)
	{
		m_fiberCtr.emplace_back(new detail::fiber_impl(m_stackAllocator,*m_fiberScheduler));
	}

	m_fiberScheduler->start();
}

}
