
#if defined(WIN32)

#include <windows.h>

#elif defined(__LINUX__) || defined(__APPLE__)

#include <sys/mman.h>

#endif

namespace ddk
{
namespace detail
{

template<size_t NumGuardPages>
stack_alloc_const_dist_ref dynamic_stack_allocator<NumGuardPages>::share() const
{
	return ref_from_this();
}
template<size_t NumGuardPages>
stack_alloc_dist_ref dynamic_stack_allocator<NumGuardPages>::share()
{
	return ref_from_this();
}
template<size_t NumGuardPages>
void* dynamic_stack_allocator<NumGuardPages>::reserve(size_t i_size) const
{

#if defined(WIN32)

	if (void* baseAddr = VirtualAlloc(nullptr, i_size * s_pageSize, MEM_RESERVE, PAGE_NOACCESS))

#elif defined(__LINUX__) || defined(__APPLE__)

	if (void* baseAddr = mmap(nullptr, i_size * s_pageSize, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0))

#endif
	{
		return reinterpret_cast<char*>(baseAddr) + i_size * s_pageSize;
	}
	else
	{
		DDK_FAIL("Error while trying to allocate memory");

		return nullptr;
	}
}
template<size_t NumGuardPages>
std::pair<void*,void*> dynamic_stack_allocator<NumGuardPages>::allocate(void* i_ref, size_t i_size) const
{
	size_t allocSize = i_size * s_pageSize;

#if defined(WIN32)

	//publish initially one page
	void* endStackAddr = VirtualAlloc(reinterpret_cast<char*>(i_ref) - allocSize,allocSize,MEM_COMMIT,PAGE_READWRITE);

	//save one page guard
	void* deallocStackAddr = VirtualAlloc(reinterpret_cast<char*>(endStackAddr) - s_numGuardPages * s_pageSize,s_numGuardPages * s_pageSize,MEM_COMMIT,PAGE_READWRITE | PAGE_GUARD);

	return std::make_pair(endStackAddr,deallocStackAddr);

#elif defined(__LINUX__) || defined(__APPLE__)

	//publish initially one page
	void* endStackAddr = reinterpret_cast<char*>(i_ref) - allocSize;

	mprotect(stackAddr,allocSize,PROT_READ|PROT_WRITE);

	return std::make_pair(endStackAddr,endStackAddr);

#endif
}
template<size_t NumGuardPages>
bool dynamic_stack_allocator<NumGuardPages>::reallocate(detail::execution_stack& i_stackAddr, void* i_reason) const
{
    static const size_t s_guard_area = NumGuardPages * s_pageSize;

	if(reinterpret_cast<char*>(i_stackAddr.get_end()) >= i_reason && i_reason > (reinterpret_cast<char*>(i_stackAddr.get_init()) - k_maxNumStackPages * s_pageSize))
	{

#if defined(WIN32)

		if(VirtualAlloc(reinterpret_cast<char*>(i_stackAddr.get_end()) - ((s_numGuardPages + 1) * s_pageSize),s_pageSize,MEM_COMMIT,PAGE_READWRITE | PAGE_GUARD))
		{
			i_stackAddr.set_end(reinterpret_cast<char*>(i_stackAddr.get_end()) - s_pageSize);
			i_stackAddr.set_dealloc(i_stackAddr.get_end());

			return true;
		}

#elif defined(__LINUX__) or defined(__APPLE__)

        if(mprotect(reinterpret_cast<char*>(i_stackAddr.get_end()) - s_guard_area,s_guard_area,PROT_READ|PROT_WRITE) == 0)
        {
			i_stackAddr.set_end(reinterpret_cast<char*>(i_stackAddr.get_end()) - s_guard_area);
			i_stackAddr.set_dealloc(i_stackAddr.get_end());

			return true;
        }

#endif

    }

	return false;
}
template<size_t NumGuardPages>
void dynamic_stack_allocator<NumGuardPages>::deallocate(void* i_address,size_t i_size) const
{
	bool freeRes = false;

#if defined(WIN32)

	freeRes = (VirtualFree(i_address,i_size * s_pageSize,MEM_DECOMMIT) != 0);

#elif defined(__LINUX__) || defined(__APPLE__)

	freeRes = mprotect(i_address,i_size * s_pageSize,PROT_NONE) == 0;

#endif

	DDK_ASSERT(freeRes, "Error freeing virtual memory");
}
template<size_t NumGuardPages>
void dynamic_stack_allocator<NumGuardPages>::release(void* i_address,size_t i_size) const
{
	bool freeRes = false;

#if defined(WIN32)

	freeRes = (VirtualFree(reinterpret_cast<char*>(i_address) - i_size * s_pageSize,0,MEM_RELEASE) != 0);

#elif defined(__LINUX__) || defined(__APPLE__)

	freeRes = (munmap(reinterpret_cast<char*>(i_address) - i_size * s_pageSize,i_size * s_pageSize) == 0);

#endif

	DDK_ASSERT(freeRes, "Error freeing virtual memory");
}
template<size_t NumGuardPages>
size_t dynamic_stack_allocator<NumGuardPages>::get_num_guard_pages() const
{
	return NumGuardPages;
}

}

template<size_t NumGuardPages>
stack_alloc_dist_ref make_dynamic_stack_allocator()
{
	return make_distributed_reference<detail::dynamic_stack_allocator<NumGuardPages>>();
}

}
