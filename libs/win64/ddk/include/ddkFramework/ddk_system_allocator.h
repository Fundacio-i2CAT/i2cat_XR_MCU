#pragma once

#include "ddk_reference_wrapper_deleter.h"
#include <cstddef>

namespace ddk
{

class system_allocator : public resource_deleter_interface
{
public:
    typedef system_allocator allocator;
    typedef void type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef std::ptrdiff_t difference_type;

	system_allocator() = default;
    void* allocate(size_t numUnits, size_t unitSize) const;
    void* allocate(size_t unitSize) const;
    template<typename T>
    static inline void* aligned_allocate(void*& i_ptr,size_t& i_remainingSize);
    void deallocate(const void *ptr) const override;
    void* reallocate(void *ptr, size_t numUnits, size_t unitSize) const;
};

template<typename T>
class typed_system_allocator : public system_allocator
{
public:
    typedef typed_system_allocator allocator;
    typedef T type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T *const_pointer;
    typedef const T& const_reference;
    typedef ptrdiff_t difference_type;
	using system_allocator::allocate;
	using system_allocator::deallocate;
	using system_allocator::reallocate;

	typed_system_allocator() = default;
    void* allocate(size_t numUnits) const;
    void* reallocate(void *ptr, size_t numUnits) const;
};

resource_deleter_const_lent_ref get_reference_wrapper_deleter(const system_allocator&);
template<typename T>
resource_deleter_const_lent_ref get_reference_wrapper_deleter(const typed_system_allocator<T>& i_allocator);

}

#include "ddk_system_allocator.inl"
