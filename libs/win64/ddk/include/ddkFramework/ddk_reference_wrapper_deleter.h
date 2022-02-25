#pragma once

#include "ddk_tagged_pointer.h"
#include "ddk_scoped_enum.h"
#include "ddk_resource_deleter.h"
#include <utility>

#ifdef _WIN32

#pragma warning ( push )
#pragma warning ( disable: 4793 )

#endif

#define DDK_ADDRESSABLE_TYPE_BASE(_TYPE) \
 \
virtual std::ptrdiff_t resolve_pointer_offset(const void* i_ptr) const \
{ \
	return 0; \
} \
friend inline const void* allocator_address_reference_wrapper(const _TYPE* i_ptr) \
{ \
	return reinterpret_cast<const char*>(i_ptr) + i_ptr->resolve_pointer_offset(i_ptr); \
}

#define DDK_ADDRESSABLE_TYPE \
 \
virtual std::ptrdiff_t resolve_pointer_offset(const void* i_ptr) const \
{ \
	return reinterpret_cast<const char*>(this) - reinterpret_cast<const char*>(i_ptr); \
}

namespace ddk
{

SCOPED_ENUM_DECL(AllocationMode,
				AllocationOnly = 0,
				ConstructionProvided);


typedef tagged_pointer<resource_deleter_const_lent_ptr> tagged_pointer_deleter;

template<typename Allocator>
resource_deleter_const_lent_ptr get_reference_wrapper_deleter(const Allocator&, ...)
{
    static_assert(sizeof(Allocator)==0, "You shall specialize this reference wrapper allocator for this type");

    return nullptr;
}

inline const void* allocator_address_reference_wrapper(const void* i_ptr, ...)
{
	//default impl is the trivial one
	return i_ptr;
}

}

#ifdef _WIN32

#pragma warning ( pop )

#endif
