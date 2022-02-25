#pragma once

#include "ddk_thread_local_storage.h"

namespace ddk
{

template<typename T, typename Tag>
class threadlocal
{
	static const size_t k_typeSize = mpl::size_of_qualified_type<T>::value;

public:
	threadlocal() = default;
	threadlocal(const threadlocal&) = delete;
	threadlocal(threadlocal&&) = default;
	~threadlocal() = default;

	template<typename ... Args>
	T& acquire(Args&& ... i_args);
	template<typename ... Args>
	T& set(Args&& ... i_args);
	T& get();
	T* get_ptr();
	T extract();
	bool empty() const;
	void clear();

private:
	inline thread_local_storage<T>& get_address() const;
};

}

#include "ddk_thread_local.inl"
