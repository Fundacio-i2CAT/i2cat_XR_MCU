
#include "ddk_embedded_type.h"
#include "ddk_async_exceptions.h"
#include "ddk_lock_guard.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename T>
private_async_state<T>::reference_counter::reference_counter(private_async_state& i_asyncSharedState)
: m_asyncSharedState(i_asyncSharedState)
{
}
template<typename T>
unsigned int private_async_state<T>::reference_counter::decrementSharedReference()
{
	unsigned int res = 0;

	m_asyncSharedState.detach_if([&]() mutable
	{
		res = distributed_reference_counter::decrementSharedReference();

		//in case we descend to 1 reference (that of the promise), please trigger its execution by means of removing our reference of async execution (check async_executor destructor).
		return res == 1;
	});
		
	return res;
}

template<typename T>
private_async_state<T>::private_async_state()
: m_arena(none)
, m_mutex(MutexType::Recursive)
, m_refCounter(*this)
{
}
template<typename T>
private_async_state<T>::~private_async_state()
{
	//just to avoid races on destruction
	mutex_guard lg(m_mutex);
}
template<typename T>
typename private_async_state<T>::cancel_result private_async_state<T>::cancel()
{
	mutex_guard lg(m_mutex);

	if(m_asyncExecutor)
	{
		return m_asyncExecutor->cancel();
	}
	else
	{
		return make_error<cancel_result>(async_cancellable_interface::CancelErrorCode::CancelNoAsync);
	}
}
template<typename T>
void private_async_state<T>::attach(async_cancellable_dist_ptr i_executor)
{
	mutex_guard lg(m_mutex);

	m_asyncExecutor = i_executor;
}
template<typename T>
void private_async_state<T>::detach()
{
	mutex_guard lg(m_mutex);

	m_asyncExecutor = nullptr;
}
template<typename T>
template<typename Predicate>
bool private_async_state<T>::detach_if(Predicate&& i_predicate)
{
	mutex_guard lg(m_mutex);

	if(ddk::eval(std::forward<Predicate>(i_predicate)))
	{
		m_asyncExecutor = nullptr;

		return true;
	}
	else
	{
		return false;
	}
}
template<typename T>
async_base_dist_ptr private_async_state<T>::get_aync_execution() const
{
	mutex_guard lg(m_mutex);

	return m_asyncExecutor;
}
template<typename T>
void private_async_state<T>::set_value(sink_type i_value)
{
	mutex_guard lg(m_mutex);

	m_arena = std::forward<sink_type>(i_value);

	m_asyncExecutor = nullptr;

	m_condVar.notify_all();
}
template<typename T>
void private_async_state<T>::set_exception(const async_exception& i_exception)
{
	mutex_guard lg(m_mutex);

	m_arena = i_exception;

	m_asyncExecutor = nullptr;

	m_condVar.notify_all();
}
template<typename T>
void private_async_state<T>::signal() const
{
	m_condVar.notify_all();
}
template<typename T>
typename private_async_state<T>::const_reference private_async_state<T>::get_value() const
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		notify();
	}

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait(m_mutex);
	}

	if(m_arena.template is<T>())
	{
		return m_arena.template get<T>();
	}
	else if(m_arena.template is<async_exception>())
	{
		throw m_arena.template get<async_exception>();
	}
	else
	{
		throw async_exception("Accessing empty async shared state");
	}
}
template<typename T>
typename private_async_state<T>::reference private_async_state<T>::get_value()
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		notify();
	}

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait(m_mutex);
	}

	if(m_arena.template is<T>())
	{
		return m_arena.template get<T>();
	}
	else if(m_arena.template is<async_exception>())
	{
		throw m_arena.template get<async_exception>();
	}
	else
	{
		throw async_exception("Accessing empty async shared state");
	}
}
template<typename T>
embedded_type<T> private_async_state<T>::extract_value()
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		notify();
	}

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait(m_mutex);
	}

	if(m_arena.template is<T>())
	{
		embedded_type<T> res = std::move(m_arena).template extract<T>();

		m_arena = none;

		return std::move(res);
	}
	else if(m_arena.template is<async_exception>())
	{
		throw std::move(m_arena).template extract<async_exception>();
	}
	else
	{
		throw async_exception("Accessing empty async shared state");
	}
}
template<typename T>
void private_async_state<T>::wait() const
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		notify();
	}

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait(m_mutex);
	}
}
template<typename T>
void private_async_state<T>::wait_for(unsigned int i_period) const
{
	mutex_guard lg(m_mutex);

	if(m_arena.template is<detail::none_t>())
	{
		notify();
	}

	if(m_arena.template is<detail::none_t>())
	{
		m_condVar.wait(m_mutex,std::chrono::seconds(i_period));
	}
}
template<typename T>
bool private_async_state<T>::ready() const
{
	mutex_guard lg(m_mutex);

	return (m_arena.template is<detail::none_t>() == false);
}
template<typename T>
void private_async_state<T>::notify() const
{
	mutex_guard lg(m_mutex);

	if(m_asyncExecutor)
	{
		m_asyncExecutor->notify();
	}
}

}
}
