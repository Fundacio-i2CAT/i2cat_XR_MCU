#pragma once

#include "ddk_fixed_size_allocator.h"
#include "ddk_system_allocator.h"

#define DDK_APPEND_GLOBAL_ALLOCATOR(...) \
namespace ddk \
{ \
	bool g__appended_global_allocs = __append_global_allocator_map_entries({__VA_ARGS__}); \
}

namespace ddk
{

template<typename Allocator>
class fixed_size_allocate_or
{
public:
	fixed_size_allocate_or(size_t i_unitSize, const Allocator& i_fallbackAllocator);
	fixed_size_allocate_or(size_t i_unitSize, Allocator&& i_fallbackAllocator);

	std::pair<resource_deleter_const_lent_ref,void*> allocate(size_t unitSize) const;
	void deallocate(const std::pair<resource_deleter_const_lent_ref,const void*>& ptr) const;

private:
	const fixed_size_allocator* m_primaryAllocator = nullptr;
	Allocator m_secondaryAllocator;
};

//null deleter
class null_deleter: public resource_deleter_interface
{
public:
	void deallocate(const void* i_object) const final;
};

extern const ddk::null_deleter g_nullDeleter;

}

#include "ddk_global_allocators.inl"