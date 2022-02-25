#pragma once

#include "ddk_atomics.h"

namespace ddk
{

class spin_lock
{
public:
	spin_lock();

	void lock();
	void unlock();
	bool try_lock();
	bool is_locked() const;

private:
	mutable atomic_bool m_barrier;
};

class exclusive_spin_lock
{
	static const size_t s_waiting_writer_bit = ~(static_cast<size_t>(-1) >> 1);
	static const size_t s_writer_bit = s_waiting_writer_bit >> 1;
	static const size_t s_writer_mask = s_waiting_writer_bit | s_writer_bit;

public:
	exclusive_spin_lock();

	void lock();
	void unlock();
	void lock_exclusive(bool i_withPriority = false);
	void unlock_exclusive();
	bool is_locked() const;

private:
	mutable atomic_size_t m_barrier;
};

}
