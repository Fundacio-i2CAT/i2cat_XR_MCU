#pragma once

#include "ddk_function.h"
#include "ddk_thread_utils.h"
#include "ddk_mutex.h"
#include "ddk_cond_var.h"
#include "ddk_atomic_shared_pointer_wrapper.h"
#include "ddk_atomic_weak_pointer_wrapper.h"

#define WATCHABLE_TYPE(_TYPE) \
mutable ddk::watch_dog_waiter_weak_ptr __watch_dog; \
friend inline ddk::watch_dog make_watcher(const _TYPE& i_watchable) \
{ \
	return ddk::watch_dog(i_watchable.__watch_dog); \
} \
friend inline void watch(const _TYPE& i_watchable) \
{ \
	ddk::watch_dog watcher(i_watchable.__watch_dog); \
	\
	watcher.watch(); \
} \
friend inline bool is_watchable(const _TYPE&) \
{ \
	return true; \
} \
friend inline bool is_watched(const _TYPE& i_watchable) \
{ \
	return i_watchable.__watch_dog != nullptr; \
} \
inline void notify_watcher() \
{ \
	if(ddk::watch_dog_waiter_shared_ptr waiterSharedPtr = share(__watch_dog)) \
	{ \
		waiterSharedPtr->notify_one(); \
	} \
} \
inline void notify_watchers() \
{ \
	if(ddk::watch_dog_waiter_shared_ptr waiterSharedPtr = share(__watch_dog)) \
	{ \
		waiterSharedPtr->notify_all(); \
	} \
}

namespace ddk
{
namespace detail
{

class watch_dog_waiter
{
public:
	watch_dog_waiter() = default;

	void lock();
	void unlock();
	void wait();
	void wait_until(const function<bool()>& i_predicate);
	void notify_one();
	void notify_all();

private:
	mutex m_mutex;
	cond_var m_condVar;
};

}

typedef atomic_shared_pointer_wrapper<detail::watch_dog_waiter> watch_dog_waiter_shared_ptr;
typedef atomic_weak_pointer_wrapper<detail::watch_dog_waiter> watch_dog_waiter_weak_ptr;

class watch_dog
{
public:
	watch_dog(watch_dog_waiter_weak_ptr& i_weakWaiter);

	void watch();
	void watch_until(const function<bool()>& i_predicate);
	void notify_one();
	void notify_all();

private:
	watch_dog_waiter_shared_ptr m_waiter;
};

}

template<typename T>
inline ddk::watch_dog watch(const T&);
template<typename T>
inline bool is_watchable(const T&);
template<typename T>
inline bool is_watched(const T&);

#include "ddk_watch_dog.inl"