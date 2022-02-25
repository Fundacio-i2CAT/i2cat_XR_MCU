#pragma once

#include <unordered_map>
#include "ddk_fiber_defs.h"
#include "ddk_thread_local_storage.h"

namespace ddk
{

template<typename T>
class fiber_local_storage
{
public:
	fiber_local_storage() = default;

	template<typename ... Args>
	T* construct(const fiber_id& i_id, Args&& ... i_args);
	inline void destroy(const fiber_id& i_id);
	template<typename ... Args>
	inline T* assign(const fiber_id& i_id, Args&& ... i_args);
	void clear(const fiber_id& i_id);
	T& get(const fiber_id& i_id);
	T extract(const fiber_id& i_id);
	bool empty(const fiber_id& i_id) const;

private:
	std::unordered_map<fiber_id,T> m_fiberStorage;
};

}

#include "ddk_fiber_local_storage.inl"
