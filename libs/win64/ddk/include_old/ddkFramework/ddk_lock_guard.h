#pragma once

#include "ddk_mutex.h"
#include "ddk_template_helper.h"

namespace ddk
{
namespace detail
{

template<typename,typename ... TT>
class lock_guard_impl;

template<typename T>
class lock_guard_impl<mpl::sequence<>,T>
{
public:
	lock_guard_impl(T& i_lockableObject);
	~lock_guard_impl();

private:
	T& m_lockableObject;
};

template<size_t ... Indexs,typename T, typename ... TT>
class lock_guard_impl<mpl::sequence<Indexs...>,T,TT...>
{
    static const size_t s_numSecondaryLocks = mpl::get_num_types<TT...>();

public:
	lock_guard_impl(T& i_lockableObject, TT& ... i_lockableObjects);
	~lock_guard_impl();

private:
    T& m_primaryLock;
	void* m_secondaryLocks[s_numSecondaryLocks] = { nullptr };
};

}

template<typename T, typename ... TT>
using lock_guard = detail::lock_guard_impl<typename mpl::make_sequence<0,mpl::get_num_types<TT...>()>::type,T,TT...>;

typedef lock_guard<mutex> mutex_guard;

}

#include "ddk_lock_guard.inl"
