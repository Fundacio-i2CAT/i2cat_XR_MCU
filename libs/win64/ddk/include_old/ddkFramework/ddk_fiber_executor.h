#pragma once

#include "ddk_fiber.h"
#include "ddk_thread_executor_interface.h"
#include "ddk_mutex.h"
#include "ddk_cond_var.h"

namespace ddk
{

class fiber_polling_executor : public thread_executor_interface
{
public:
	fiber_polling_executor(ddk::fiber i_fiber, unsigned long i_sleepInMs = 0);
	fiber_polling_executor(const fiber_polling_executor&) = delete;
	fiber_polling_executor(fiber_polling_executor&& other);
	~fiber_polling_executor();
	void set_update_time(const std::chrono::milliseconds& i_sleepInMs);
	std::chrono::milliseconds get_update_time() const;
	void start(const ddk::function<void()>& i_executor);
	void stop();

private:
	virtual start_result execute(const sink_type& i_sink, const ddk::function<void()>& i_executor) override;
	virtual bool pending() const override;
	virtual resume_result resume() override;
	virtual void signal() override;
	void update();

	ddk::fiber m_fiber;
	std::chrono::milliseconds m_sleepTimeInMS;
	ddk::function<void()> m_executor;
	bool m_stopped;
};

class fiber_event_driven_executor : public thread_executor_interface
{
public:
	fiber_event_driven_executor(ddk::fiber i_fiber, unsigned int i_sleepInMs = 0);
	fiber_event_driven_executor(const fiber_event_driven_executor&) = delete;
	fiber_event_driven_executor(fiber_event_driven_executor&& other);
	~fiber_event_driven_executor();
	void set_update_time(const std::chrono::milliseconds& i_sleepInMs);
	std::chrono::milliseconds get_update_time() const;
	void start(const ddk::function<void()>& i_executor, const ddk::function<bool()>& i_testFunc = nullptr);
	void stop();

private:
	virtual start_result execute(const sink_type& i_sink, const ddk::function<void()>& i_executor) override;
	virtual bool pending() const override;
	virtual resume_result resume() override;
	virtual void signal() override;
	void update();

	ddk::fiber m_fiber;
	std::chrono::milliseconds m_sleepTimeInMS;
	bool m_stopped;
	ddk::function<void()> m_executor;
	ddk::function<bool()> m_testFunc;
	cond_var m_condVar;
	mutex m_condVarMutex;
	bool m_pendingWork;
};

class fiber_fire_and_forget_executor : public thread_executor_interface
{
public:
	fiber_fire_and_forget_executor(ddk::fiber i_fiber);
	fiber_fire_and_forget_executor(const fiber_fire_and_forget_executor&) = delete;
	fiber_fire_and_forget_executor(fiber_fire_and_forget_executor&& other);
	~fiber_fire_and_forget_executor();

private:
	start_result execute(const sink_type& i_sink, const ddk::function<void()>& i_executor) override;
	virtual bool pending() const override;
	resume_result resume() override;
	void signal() override;

	ddk::fiber m_fiber;
};

}