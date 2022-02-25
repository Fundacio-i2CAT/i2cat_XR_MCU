#pragma once

#include <pthread.h>
#include "ddk_mutex.h"
#include "ddk_function.h"
#include <chrono>

namespace ddk
{

class cond_var
{
public:
	cond_var();
	~cond_var();

	void wait(mutex& i_mutex);
	void wait(mutex& i_mutex,const std::chrono::nanoseconds& i_time);
	void wait_until(mutex& i_mutex, const function<bool()>& i_predicate);
	void wait_until(mutex& i_mutex, const function<bool()>& i_predicate, const std::chrono::nanoseconds& i_time);
	void notify_all();
	void notify_one();

private:
	mutable pthread_cond_t m_condVar;
};

}