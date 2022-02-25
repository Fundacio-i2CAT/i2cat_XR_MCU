#pragma once

#include "ddk_ucontext.h"
#include "ddk_reference_wrapper.h"
#include "ddk_thread_impl.h"
#include "ddk_async_exceptions.h"
#include "ddk_lock_guard.h"

namespace ddk
{
namespace detail
{

template<typename Comparator>
bool priority_queue<Comparator>::has_item(const fiber_id& i_id) const
{
	return std::find(this->c.begin(), this->c.end(),i_id) != this->c.end();
}

}

template<typename Comparator>
fiber_scheduler<Comparator>::fiber_scheduler()
: m_stop(false)
{
}
template<typename Comparator>
fiber_scheduler<Comparator>::~fiber_scheduler()
{
	DDK_ASSERT(m_fibers.empty(), "Destroying fiber scheduler with pending fibers");

	stop();
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::register_fiber_result fiber_scheduler<Comparator>::register_fiber(fiber& i_fiber)
{
	mutex_guard lg(m_fiberMutex);

	fiber_container::const_iterator itFiber = m_fibers.find(i_fiber.get_id());

	if(itFiber == m_fibers.end())
	{
		i_fiber.m_impl->set_executor(this->ref_from_this());

		const std::pair<fiber_container::const_iterator,bool> insertRes = m_fibers.insert(std::make_pair(i_fiber.get_id(),i_fiber.m_impl.get()));

		if(insertRes.second)
		{
			return success;
		}
		else
		{
			return make_error<register_fiber_result>(InternalError);
		}
	}
	else
	{
		return make_error<register_fiber_result>(FiberAlreadyRegistered);
	}
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::unregister_fiber_result fiber_scheduler<Comparator>::unregister_fiber(fiber_id i_id)
{
	mutex_guard lg(m_fiberMutex);

	fiber_container::iterator itFiber = m_fibers.find(i_id);

	if(itFiber != m_fibers.end())
	{
		itFiber->second->set_executor(nullptr);

		m_fibers.erase(itFiber);

		return success;
	}
	else
	{
		return make_error<unregister_fiber_result>(UnexistentFiber);
	}
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::iterator fiber_scheduler<Comparator>::begin()
{
	return m_fibers.begin();
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::iterator fiber_scheduler<Comparator>::end()
{
	return m_fibers.end();
}
template<typename Comparator>
void fiber_scheduler<Comparator>::resize(size_t i_size)
{
	mutex_guard lg(m_fiberMutex);

	m_fibers.reserve(i_size);
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::const_iterator fiber_scheduler<Comparator>::begin() const
{
	return m_fibers.begin();
}
template<typename Comparator>
typename fiber_scheduler<Comparator>::const_iterator fiber_scheduler<Comparator>::end() const
{
	return m_fibers.end();
}
template<typename Comparator>
bool fiber_scheduler<Comparator>::empty() const
{
	return m_fibers.empty();
}
template<typename Comparator>
size_t fiber_scheduler<Comparator>::size() const
{
	return m_fibers.size();
}
template<typename Comparator>
void fiber_scheduler<Comparator>::start()
{
	m_fiberThread.start(ddk::make_function(this,&fiber_scheduler<Comparator>::run));
}
template<typename Comparator>
void fiber_scheduler<Comparator>::stop()
{
	if(m_fiberThread.joinable())
	{
		//cleanup
		m_fiberMutex.lock();

		m_functions.clear();

		while (m_runningFibers.empty() == false)
		{
			detail::running_fiber topFiber = m_runningFibers.top();

			m_fiberMutex.unlock();

			topFiber->stop();

			m_fiberMutex.lock();
		}

		m_stop = true;

		m_fiberCondVar.notify_one();;

		m_fiberMutex.unlock();

		m_fiberThread.stop();
	}
}
template<typename Comparator>
void fiber_scheduler<Comparator>::yield(detail::yielder_context* i_context)
{
	m_callee.set_last_time(time(NULL));

	m_runningFibers.push(++m_callee);

	m_callee->resume_to(m_caller);
}
template<typename Comparator>
void fiber_scheduler<Comparator>::suspend(detail::yielder_context* i_context)
{
	{
		mutex_guard lg(m_fiberMutex);

		m_fiberCondVar.notify_one();
	}

	throw suspend_exception(m_callee->get_id());
}
template<typename Comparator>
bool fiber_scheduler<Comparator>::activate(fiber_id i_id, const ddk::function<void()>& i_function)
{
	mutex_guard lg(m_fiberMutex);

	fiber_container::iterator itFiber = m_fibers.find(i_id);
	if(itFiber != m_fibers.end())
	{
		std::pair<function_container::iterator,bool> insertRes = m_functions.insert(std::make_pair(i_id,i_function));

		m_fiberCondVar.notify_one();

		return true;
	}
	else
	{
		DDK_FAIL("Trying to activate not registered fiber");

		return false;
	}
}
template<typename Comparator>
bool fiber_scheduler<Comparator>::deactivate(fiber_id i_id)
{
	mutex_guard lg(m_fiberMutex);

	function_container::iterator itFunction = m_functions.find(i_id);
	if(itFunction != m_functions.end())
	{
		m_functions.erase(itFunction);

		return true;
	}
	else
	{
		m_fiberCondVar.wait_until(m_fiberMutex,make_function([this,i_id](){ return m_runningFibers.has_item(i_id); }));

		return false;
	}
}
template<typename Comparator>
void fiber_scheduler<Comparator>::unregister(fiber_id i_id)
{
	mutex_guard lg(m_fiberMutex);

	fiber_container::iterator itFiber = m_fibers.find(i_id);

	if(itFiber != m_fibers.end())
	{
		m_fibers.erase(itFiber);
	}
	else
	{
		DDK_FAIL("Trying to unregister not present fiber");
	}
}
template<typename Comparator>
void fiber_scheduler<Comparator>::run()
{
	while(m_stop == false)
	{
		ddk::function<void()> callableObject;

		m_fiberMutex.lock();

		function_container::iterator itFunction = m_functions.begin();
		if(itFunction != m_functions.end())
		{
			fiber_container::iterator itFiber = m_fibers.find(itFunction->first);
			if(itFiber != m_fibers.end())
			{
				if(detail::fiber_impl* currFiber = itFiber->second)
				{
					callableObject = itFunction->second;

					m_functions.erase(itFunction);

					m_fiberMutex.unlock();

					currFiber->start_from(m_caller,callableObject);

					m_fiberMutex.lock();

					m_runningFibers.emplace(currFiber,0);

					m_fiberMutex.unlock();
				}
				else
				{
					m_functions.erase(itFunction);

					m_fiberMutex.unlock();
				}
			}
			else
			{
				m_fiberMutex.unlock();
			}
		}
		else
		{
			m_fiberMutex.unlock();
		}

		m_fiberMutex.lock();

		if(m_runningFibers.empty() == false)
		{
			m_callee = m_runningFibers.top();

			m_runningFibers.pop();

			m_fiberMutex.unlock();

			m_callee->resume_from(m_caller);
		}
		else
		{
			if (m_stop == false)
			{
				m_fiberCondVar.wait(m_fiberMutex);
			}

			m_fiberMutex.unlock();
		}
	}
}

}
