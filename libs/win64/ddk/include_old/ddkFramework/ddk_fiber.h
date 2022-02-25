#pragma once

#include "ddk_lent_reference_wrapper.h"
#include "ddk_fiber_impl.h"
#include "ddk_fiber_defs.h"

namespace ddk
{

class fiber
{
	template<typename>
	friend class fiber_scheduler;

public:
	fiber(detail::fiber_impl_unique_ref i_fiber);
	fiber(const fiber&) = delete;
	fiber(fiber&& other);
	~fiber() = default;
	fiber& operator=(const fiber&) = delete;
	fiber& operator=(fiber&& other);

	fiber_id get_id() const;
	void start(const ddk::function<void()>& i_function);
	bool ready() const;
	void stop();
	bool joinable() const;

private:
	detail::fiber_impl_unique_ref m_impl;
};

//extern const thread_local detail::this_fiber_t this_fiber;

template<typename Allocator, typename ... Args>
fiber make_fiber(Args&& ... i_args);

}

#include "ddk_fiber.inl"
