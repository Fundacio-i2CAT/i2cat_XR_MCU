#pragma once

#include "ddk_sync_executor_context_interface.h"
#include "ddk_mutex.h"
#include "ddk_thread.h"
#include "ddk_fiber.h"
#include "ddk_thread_sheaf.h"
#include "ddk_fiber_sheaf.h"
#include "ddk_lend_from_this.h"
#include <queue>
#include <list>

namespace ddk
{
namespace detail
{

struct async_executor_recipients
{

struct task
{
	friend inline void eval(const task& i_task)
	{
		eval(i_task.m_callable);
	}

public:
	task(size_t i_token, const function<void()>& i_callable);

	bool operator==(const continuation_token& i_token) const;

private:
	size_t m_token;
	function<void()> m_callable;
};

	typedef std::map<unsigned char,std::list<task>> callable_container;

public:
	async_executor_recipients() = default;

	void notify();
	continuation_token accept(const function<void()>&, unsigned char i_depth);
	bool dismiss(unsigned char i_depth, continuation_token i_token);
	void clear();

protected:
	mutex m_mutex;
	callable_container m_pendingCallables;
	bool m_admissible = true;
};

}

class deferred_execution_context : public executor_context_interface, public lend_from_this<deferred_execution_context,executor_context_interface>
{
public:
	void start(const function<void()>&) override;

private:
	continuation_token enqueue(const function<void()>&, unsigned char i_depth) override;
	bool dismiss(unsigned char i_depth,continuation_token i_token) override;
	void clear() override;
};

class thread_execution_context : public executor_context_interface, public lend_from_this<thread_execution_context,executor_context_interface>
{
public:
	thread_execution_context(thread i_thread);
	void start(const function<void()>&) override;

private:
	continuation_token enqueue(const function<void()>&, unsigned char i_depth) override;
	bool dismiss(unsigned char i_depth,continuation_token i_token) override;
	void clear() override;

	thread m_thread;
	detail::async_executor_recipients m_recipients;
};

class fiber_execution_context : public executor_context_interface, public lend_from_this<fiber_execution_context,executor_context_interface>
{
public:
	fiber_execution_context(fiber i_fiber);

	void start(const function<void()>&) override;

private:
	continuation_token enqueue(const function<void()>&, unsigned char i_depth) override;
	bool dismiss(unsigned char i_depth,continuation_token i_token) override;
	void clear() override;

	fiber m_fiber;
	detail::async_executor_recipients m_recipients;
};

class thread_sheaf_execution_context : public executor_context_interface, public lend_from_this<thread_sheaf_execution_context,executor_context_interface>
{
public:
	thread_sheaf_execution_context(thread_sheaf i_threadSheaf);
	void notify_recipients();
	size_t add_failure();
	size_t remove_pending_fiber();
	bool has_pending_threads() const;
	bool has_failures() const;
	continuation_token enqueue(const function<void()>&);

	void start(const function<void()>&) override;

private:
	continuation_token enqueue(const function<void()>&, unsigned char i_depth) override;
	bool dismiss(unsigned char i_depth,continuation_token i_token) override;
	void clear() override;

	thread_sheaf m_threadSheaf;
	atomic_size_t m_failedThreads;
	atomic_size_t m_pendingThreads;
	detail::async_executor_recipients m_recipients;
};

class fiber_sheaf_execution_context : public executor_context_interface, public lend_from_this<fiber_sheaf_execution_context,executor_context_interface>
{
public:
	fiber_sheaf_execution_context(fiber_sheaf i_fiberSheaf);

	void clear_fibers();
	size_t add_failure();
	size_t remove_pending_thread();
	void notify_recipients();
	bool has_pending_fibers() const;
	bool has_failures() const;
	continuation_token enqueue(const function<void()>&);

	void start(const function<void()>&) override;

private:
	continuation_token enqueue(const function<void()>&, unsigned char i_depth) override;
	bool dismiss(unsigned char i_depth,continuation_token i_token) override;
	void clear() override;

	fiber_sheaf m_fiberSheaf;
	atomic_size_t m_failedFibers;
	atomic_size_t m_pendingFibers;
	detail::async_executor_recipients m_recipients;
};

}
