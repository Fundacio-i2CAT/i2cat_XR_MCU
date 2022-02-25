#pragma once

#include <pthread.h>
namespace ddk
{

#if defined(WIN32)

    #define CURR_THREAD_ID GetCurrentThreadId()
	typedef unsigned long long thread_id_t;

#elif defined(__LINUX__)

    #define CURR_THREAD_ID pthread_self()
	typedef unsigned long long thread_id_t;
    #define thread_local __thread

#elif defined(__APPLE__)

    #define CURR_THREAD_ID reinterpret_cast<size_t>(pthread_self())
    typedef size_t thread_id_t;

#endif

thread_id_t get_current_thread_id();
thread_id_t get_thread_id(pthread_t);
pthread_t get_current_thread();
void sleep(unsigned long milliseconds);
bool thread_id_equal(const thread_id_t& i_lhs, const thread_id_t& i_rhs);

}
