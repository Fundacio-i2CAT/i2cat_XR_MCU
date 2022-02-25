#pragma once

#include "ddk_thread_impl.h"
#include "ddk_thread.h"
#include "ddk_optional.h"
#include "ddk_reference_wrapper_deleter.h"
#include "ddk_thread_sheaf.h"
#include "ddk_result.h"
#include "ddk_mutex.h"
#include "ddk_cond_var.h"
#include "ddk_mutex.h"
#include "ddk_signal.h"
#include <functional>
#include <vector>

namespace ddk
{
namespace detail
{

class worker_thread_impl : public thread_impl_interface
{
public:
	worker_thread_impl();
	~worker_thread_impl();
	worker_thread_impl(const worker_thread_impl&) = delete;
	worker_thread_impl(worker_thread_impl&&) = delete;

private:
	enum State
	{
		Idle,
		Running,
		Stopped
	};

	void start(const ddk::function<void()>&, yielder* i_yielder) override;
	void stop() override;
	bool joinable() const override;
	void setExiting(bool i_exiting) override;
	void execute() override;
	bool set_affinity(const cpu_set_t& i_set) override;
	thread_id_t get_id() const override;

	pthread_t m_thread;
	mutex m_mutex;
	cond_var m_condVar;
	ddk::function<void()> m_funcToExecute;
	State m_state;
	detail::yielder* m_yielder;
};

}

class thread_pool : public resource_deleter_interface
{
	typedef std::vector<detail::thread_impl_interface*> thread_container;
	typedef std::unordered_map<const void*,detail::thread_impl_interface*> thread_in_use_container;

public:
	enum Policy
	{
		FixedSize,
		GrowsOnDemand
	};
	enum AcquireErrorCode
	{
		NoThreadAvailable
	};
	template<typename T>
	using acquire_result = result<T,AcquireErrorCode>;

	signal<void()> on_availableThreads;

	thread_pool(Policy i_policy, size_t i_initialSize);
	thread_pool(const thread_pool&) = delete;
	thread_pool(thread_pool&& other);
	~thread_pool();
	thread_pool& operator=(const thread_pool&) = delete;

	size_t set_affinity(const cpu_set_t& i_set);
	acquire_result<thread> aquire_thread();
	acquire_result<thread_sheaf> acquire_sheaf(size_t i_size);
	bool available_threads() const;

private:
	void deallocate(const void* i_object) const override;

	const Policy m_policy;
	const size_t m_maxNumThreads;
	mutable thread_container m_availableThreads;
	mutable thread_in_use_container m_underUseThreads;
	mutable mutex m_mutex;
	mutable cond_var m_condVar;
};

typedef thread_pool::acquire_result<thread> acquire_thread_result;
typedef thread_pool::acquire_result<thread_sheaf> acquire_thread_sheaf_result;

}