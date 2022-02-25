#pragma once

#include "ddk_atomics.h"
#include "ddk_lend_from_this.h"
#include "ddk_reference_wrapper_deleter.h"
#include "ddk_mutex.h"
#include <cstddef>

//#define MEM_CHECK

namespace ddk
{

class fixed_size_allocator : public resource_deleter_interface
{
	static const size_t s_numReservedEntries = 2048;
	static const std::ptrdiff_t s_invalidAddress = -1;
	static const size_t s_invalidChunk = -1;

public:
	fixed_size_allocator(size_t i_unitSize,size_t i_poolSize = s_numReservedEntries);
	fixed_size_allocator(const fixed_size_allocator&) = delete;
	fixed_size_allocator(fixed_size_allocator&& other);
	~fixed_size_allocator();

	fixed_size_allocator& operator=(const fixed_size_allocator&) = delete;
	fixed_size_allocator& operator=(fixed_size_allocator&&) = delete;
	void* allocate(size_t i_size) const;
	void deallocate(const void* i_address) const override;
	size_t unit_size() const;
	template<typename T>
	inline void* aligned_allocate(void*& i_ptr,size_t& i_remainingSize) const;

private:
	const void* is_address_referenced(const void* i_address) const;
	std::ptrdiff_t get_local_address(const char* i_address) const;

	mutable atomic_size_t m_currChunk;
	const size_t m_unitSize;
	const size_t m_poolSize;
	mutable std::vector<char> m_pool;
	mutable std::vector<size_t> m_nextChunkArr;
	#ifdef MEM_CHECK
	mutable atomic_size_t m_numCurrentAllocations = 0;
	#endif
};

}

#include "ddk_fixed_size_allocator.inl"