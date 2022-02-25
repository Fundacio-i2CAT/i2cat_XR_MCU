#pragma once

#include "ddk_mutex.h"
#include <unordered_map>

namespace ddk
{

class memory_tracker
{
	static const size_t k_maxNumberOfStacks = 16;
	static const size_t k_maxNumOfChars = 64;
	typedef void* stack_entry[k_maxNumberOfStacks];
	typedef std::unordered_map<size_t,stack_entry> stack_container;

public:
	memory_tracker() = default;
	memory_tracker(const memory_tracker&) = default;
	memory_tracker(memory_tracker&& other) = default;
	~memory_tracker();

	memory_tracker& operator=(const memory_tracker&);
	memory_tracker& operator=(memory_tracker && other);
	void register_allocation(size_t i_id);
	void unregister_allocation(size_t i_id);
	void copy_entries_from(size_t i_oldId,size_t i_newId);
	void move_entries_to(size_t i_oldId,size_t i_newId);
	void clear();

private:
	mutex m_mutex;
	stack_container m_stackTraces;
};

}