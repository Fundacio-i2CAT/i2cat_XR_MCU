
#include "ddk_macros.h"

namespace ddk
{
namespace detail
{

template<size_t Size>
static_stack_allocator<Size>::~static_stack_allocator()
{
	DDK_ASSERT(m_underUse == false, "Memory under use while destroying stack");
}
template<size_t Size>
stack_alloc_const_dist_ref static_stack_allocator<Size>::share() const
{
	return make_static_stack_allocator<Size>();
}
template<size_t Size>
stack_alloc_dist_ref static_stack_allocator<Size>::share()
{
	return make_static_stack_allocator<Size>();
}
template<size_t Size>
void* static_stack_allocator<Size>::reserve(size_t) const
{
	return reinterpret_cast<char*>(reinterpret_cast<size_t>(m_arena.template get_ptr<char>() + (Size + 1) * s_pageSize) & ~0xFFF);
}
template<size_t Size>
std::pair<void*,void*> static_stack_allocator<Size>::allocate(void* i_ref, size_t i_size) const
{
	DDK_ASSERT(m_underUse == false, "Trying to allocate in already used arena");

	m_underUse = true;

	void* endStackAddr = reinterpret_cast<char*>(i_ref) - Size * s_pageSize;

	return std::make_pair(endStackAddr,endStackAddr);
}
template<size_t Size>
bool static_stack_allocator<Size>::reallocate(execution_stack& i_stackAddr, void* i_reason) const
{
	DDK_FAIL("Static stack allocator do not offer reallocate facility");

	return false;
}
template<size_t Size>
void static_stack_allocator<Size>::deallocate(void*,size_t) const
{
	DDK_ASSERT(m_underUse, "Trying to deallocate in non used arena");

	m_underUse = false;
}
template<size_t Size>
void static_stack_allocator<Size>::release(void*,size_t) const
{
	DDK_ASSERT(m_underUse == false, "Trying to release in non used arena");
}
template<size_t Size>
size_t static_stack_allocator<Size>::get_num_guard_pages() const
{
	return 0;
}

}

template<size_t Size>
stack_alloc_dist_ref make_static_stack_allocator()
{
	return make_distributed_reference<detail::static_stack_allocator<Size>>();
}

}
