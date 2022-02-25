#pragma once

#include <pthread.h>

namespace ddk
{

enum class MutexType
{
	Normal,
	Recursive,
	ErrorCheck,
	Default = Normal
};

class mutex
{
	friend class cond_var;

public:
	mutex(MutexType i_mutexType = MutexType::Default);
	mutex(const mutex& other);
	mutex(mutex&& other);
	~mutex();

	void lock();
	bool try_lock();
	void unlock();

private:
	pthread_mutex_t* get_impl();

	pthread_mutex_t m_mutex;
};

}