#pragma once

#include "ddk_fiber_local_storage.h"

namespace ddk
{

template<typename T, typename Tag>
class fiberlocal
{
public:
	fiberlocal() = default;
	fiberlocal(const fiberlocal&) = delete;
	fiberlocal(fiberlocal&&) = default;
	~fiberlocal() = default;

	template<typename ... Args>
	inline T& acquire(Args&& ... i_args);
	template<typename ... Args>
	T& set(Args&& ... i_args);
	inline T& get();
	T* get_ptr();
	T extract();
	inline void clear();
	inline bool empty() const;

private:
	inline fiber_local_storage<T>& get_address() const;
};

}

#include "ddk_fiber_local.inl"
