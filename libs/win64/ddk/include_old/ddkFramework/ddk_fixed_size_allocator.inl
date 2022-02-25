
#include "ddk_system_allocator.h"

namespace ddk
{

template<typename T>
void* fixed_size_allocator::aligned_allocate(void*& i_ptr,size_t& i_remainingSize) const
{
	if(is_address_referenced(i_ptr))
	{
		i_remainingSize = m_unitSize;
	}

	return system_allocator::aligned_allocate<T>(i_ptr,i_remainingSize);
}

}