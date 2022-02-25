#pragma once

#include "ddk_atomics.h"
#include "ddk_arena.h"
#include "ddk_tagged_pointer.h"
#include <type_traits>
#include <cstring>

namespace ddk
{

struct divider_node_t{};
const divider_node_t divider_node = divider_node_t();
struct leaf_node_t{};
const leaf_node_t leaf_node = leaf_node_t();

template<typename T>
struct lock_free_stack_node
{
public:
	lock_free_stack_node(const leaf_node_t&);
	lock_free_stack_node(const divider_node_t&);
	template<typename ... Args>
	lock_free_stack_node(Args&& ... i_args);
	~lock_free_stack_node();
	void set_next(lock_free_stack_node<T>* i_nextNode);
	bool compareAndExchangeNextNode(lock_free_stack_node<T>* i_oldNode, lock_free_stack_node<T>* i_newNode);
	const lock_free_stack_node<T>* get_next() const;
	lock_free_stack_node<T>* get_next();
	template<typename TT>
	void set_value(TT&& i_value);
	inline const T& get_value() const;
	inline T extract_value();
	inline void set_divider(bool i_divider);
	inline bool is_divider() const;
	inline bool is_void() const;

private:
	arena<sizeof(T), std::alignment_of<T>::value> m_arena;
	ddk::atomic<lock_free_stack_node<T>*> m_nextNode;
};

}

#include "ddk_lock_free_stack_node.inl"