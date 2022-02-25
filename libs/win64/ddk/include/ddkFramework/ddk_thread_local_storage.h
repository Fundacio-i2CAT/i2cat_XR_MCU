#pragma once

#include "ddk_system_allocator.h"

namespace ddk
{

template<typename T>
class thread_local_storage
{
public:
	thread_local_storage() = default;
	thread_local_storage(thread_local_storage&& other);
	~thread_local_storage();
	template<typename ... Args>
	inline T* construct(Args&& ... i_args);
	inline void destroy();
	template<typename ... Args>
	inline T* assign(Args&& ... i_args);
	T& get();
	T* get_address();
	T extract();
	bool empty() const;

private:
	system_allocator m_alloc;
	void* m_address = nullptr;
};

}

#include "ddk_thread_local_storage.inl"
