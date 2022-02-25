#pragma once

#include "ddk_mutex.h"
#include "ddk_cond_var.h"
#include "ddk_thread_utils.h"
#include "ddk_executor_interface.h"
#include "ddk_fiber_scheduler_interface.h"
#include "ddk_fiber_impl.h"
#include "ddk_fiber.h"
#include "ddk_fiber_yielder.h"
#include "ddk_thread.h"
#include "ddk_lend_from_this.h"
#include <unordered_map>
#include <stack>
#include <queue>

namespace ddk
{
namespace detail
{

struct running_fiber
{
public:
	running_fiber() = default;
	running_fiber(detail::fiber_impl* i_fiberImpl, time_t i_time);

	void set_last_time(time_t i_lastDuration);
	time_t get_last_time() const;
	time_t get_total_duration_time() const;
	size_t get_num_executions() const;
	running_fiber& operator++();
	bool operator==(fiber_id i_id) const;
	bool operator<(const running_fiber& other) const;
	detail::fiber_impl* operator->();
	const detail::fiber_impl* operator->() const;

private:
	detail::fiber_impl* m_fiber = nullptr;
	time_t m_lastExecutionTime = 0;
	time_t m_totalExecution = 0;
	size_t m_numOfExecutions = 0;
};

template<typename Comparator>
struct priority_queue : public mpl::which_type<std::is_same<void,Comparator>::value,std::stack<detail::running_fiber, std::vector<detail::running_fiber>>,std::priority_queue<detail::running_fiber, std::vector<detail::running_fiber>,Comparator>>::type
{
public:
	bool has_item(const fiber_id& i_id) const;
};

}

struct running_fiber_comparator
{
	bool operator()(const detail::running_fiber& i_lhs, detail::running_fiber& i_rhs) const;
};

template<typename Comparator = void>
class fiber_scheduler : public detail::scheduler_interface, public detail::yielder_interface, protected lend_from_this<fiber_scheduler<Comparator>, detail::scheduler_interface>
{
    friend class lend_from_this<fiber_scheduler<Comparator>, detail::scheduler_interface>;
	typedef std::unordered_map<fiber_id,detail::fiber_impl*> fiber_container;
	typedef detail::priority_queue<Comparator> running_fiber_container;
	typedef std::map<fiber_id,ddk::function<void()>> function_container;

public:
	typedef fiber_container::iterator iterator;
	typedef fiber_container::const_iterator const_iterator;
	enum RegisterFiberErrorCode
	{
		InternalError,
		FiberAlreadyRegistered
	};
	typedef result<void,RegisterFiberErrorCode> register_fiber_result;

	enum UnregisterFiberErrorCode
	{
		UnexistentFiber
	};
	typedef result<void,UnregisterFiberErrorCode> unregister_fiber_result;

	fiber_scheduler();
	~fiber_scheduler();

	void start();
	void stop();
	register_fiber_result register_fiber(fiber& i_fiber);
	unregister_fiber_result unregister_fiber(fiber_id i_id);
	iterator begin();
	iterator end();
	void resize(size_t i_size);
	const_iterator begin() const;
	const_iterator end() const;
	bool empty() const;
	size_t size() const;

private:
	void yield(detail::yielder_context*) override;
	void suspend(detail::yielder_context*) override;
	bool activate(fiber_id i_id, const ddk::function<void()>& i_function) override;
	bool deactivate(fiber_id i_id) override;
	void unregister(fiber_id i_id) override;

	void run();

	function_container m_functions;
	fiber_container m_fibers;
	running_fiber_container m_runningFibers;
	ddk::thread m_fiberThread;
	detail::this_fiber_t m_caller;
	detail::running_fiber m_callee;
	bool m_stop;
	mutex m_fiberMutex;
	cond_var m_fiberCondVar;
};

template<typename Comparator = void>
using fiber_scheduler_dist_ref = distributed_reference_wrapper<fiber_scheduler<Comparator>>;
template<typename Comparator = void>
using fiber_scheduler_const_dist_ref = distributed_reference_wrapper<const fiber_scheduler<Comparator>>;
template<typename Comparator = void>
using fiber_scheduler_dist_ptr = distributed_pointer_wrapper<fiber_scheduler<Comparator>>;
template<typename Comparator = void>
using fiber_scheduler_const_dist_ptr = distributed_pointer_wrapper<const fiber_scheduler<Comparator>>;

template<typename Comparator = void>
using fiber_scheduler_lent_ref = lent_reference_wrapper<fiber_scheduler<Comparator>>;
template<typename Comparator = void>
using fiber_scheduler_const_lent_ref = lent_reference_wrapper<const fiber_scheduler<Comparator>>;
template<typename Comparator = void>
using fiber_scheduler_lent_ptr = lent_pointer_wrapper<fiber_scheduler<Comparator>>;
template<typename Comparator = void>
using fiber_scheduler_const_lent_ptr = lent_pointer_wrapper<const fiber_scheduler<Comparator>>;

}

#include "ddk_fiber_scheduler.inl"
