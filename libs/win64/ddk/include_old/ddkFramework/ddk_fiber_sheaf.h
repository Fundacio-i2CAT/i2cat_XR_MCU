#pragma once

#include "ddk_fiber.h"
#include "ddk_optional.h"

namespace ddk
{

class fiber_pool;

class fiber_sheaf
{
	friend class fiber_pool;
	typedef std::vector<fiber> fiber_container;

public:
	typedef fiber_container::iterator iterator;
	typedef fiber_container::const_iterator const_iterator;

	fiber_sheaf(const fiber_sheaf&) = delete;
	fiber_sheaf(fiber_sheaf&& other);
	fiber_sheaf& operator=(const fiber_sheaf&) = delete;
	fiber_sheaf& operator=(fiber_sheaf&& other);
	void start(const ddk::function<void()>& i_function);
	void stop();
	void insert(fiber i_fiber);
	optional<fiber> extract();
	void clear();
	bool empty() const;
	size_t size() const;

private:
	fiber_sheaf() = default;

	fiber_container m_fiberCtr;
};

}