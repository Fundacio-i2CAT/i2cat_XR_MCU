#pragma once

#include "ddk_linked_list_node.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_list_iterator.h"
#include "ddk_system_allocator.h"
#include "ddk_optional.h"

namespace ddk
{

template<typename T, typename Allocator = typed_system_allocator<T>>
struct linked_list
{
	typedef lent_pointer_wrapper<detail::linked_list_node<T>> linked_node_ptr;
	typedef shared_reference_wrapper<detail::linked_list_node<T>> linked_node_shared_ref;
	typedef shared_pointer_wrapper<detail::linked_list_node<T>> linked_node_shared_ptr;
	typedef lent_reference_wrapper<detail::linked_list_node<T>> linked_node_lent_ref;
	typedef lent_reference_wrapper<const detail::linked_list_node<T>> linked_node_const_lent_ref;
	typedef lent_pointer_wrapper<detail::linked_list_node<T>> linked_node_lent_ptr;
	typedef lent_pointer_wrapper<const detail::linked_list_node<T>> linked_node_const_lent_ptr;

public:
	typedef list_iterator<linked_node_lent_ptr> iterator;
	typedef list_iterator<linked_node_const_lent_ptr> const_iterator;

	linked_list() = default;
	linked_list(const linked_list<T>& other);
	linked_list(linked_list<T>&& other);
	~linked_list();
	T& push();
	template<typename ... Args>
	T& push(Args&& ... i_args);
	optional<T> pop();
	void clear();
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	iterator erase(iterator i_it);
	size_t size() const;
	bool empty() const;

private:
	T& _push(linked_node_shared_ref i_node);

	linked_node_shared_ptr m_firstNode;
	linked_node_shared_ptr m_lastNode;
	Allocator m_allocator;
};

}

#include "ddk_linked_list.inl"
