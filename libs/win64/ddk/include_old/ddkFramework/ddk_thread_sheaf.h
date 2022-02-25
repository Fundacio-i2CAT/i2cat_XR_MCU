#pragma once

#include "ddk_thread.h"
#include <vector>
#include "ddk_optional.h"

namespace ddk
{

class thread_sheaf
{
	typedef std::vector<thread> thread_container;
	friend class thread_pool;

public:
	typedef thread_container::iterator iterator;
	typedef thread_container::const_iterator const_iterator;

	thread_sheaf(const thread_sheaf&) = delete;
	thread_sheaf(thread_sheaf&& other);
	thread_sheaf& operator=(const thread_sheaf&) = delete;
	thread_sheaf& operator=(thread_sheaf&& other);
	void start(const ddk::function<void()>& i_function);
	size_t start(const ddk::function<void()>& i_function, size_t i_numThreads);
	void stop();
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	void insert(thread i_thread);
	optional<thread> extract();
	void clear();
	size_t size() const;
	bool empty();

private:
	thread_sheaf() = default;

	thread_container m_threadCtr;
};


}