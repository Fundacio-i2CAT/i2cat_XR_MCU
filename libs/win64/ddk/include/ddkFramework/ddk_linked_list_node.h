#pragma once

#include "ddk_macros.h"
#include "ddk_shared_pointer_wrapper.h"
#include "ddk_lent_pointer_wrapper.h"
#include "ddk_reference_wrapper.h"
#include "ddk_shared_from_this.h"

namespace ddk
{

template<typename,typename>
struct linked_list;

namespace detail
{

template<typename T>
struct linked_list_node : public share_from_this<linked_list_node<T>>
{
	template<typename,typename>
	friend struct ::ddk::linked_list;
	typedef lent_reference_wrapper<detail::linked_list_node<T>> linked_node_ref;
	typedef lent_reference_wrapper<const detail::linked_list_node<T>> linked_node_const_ref;
	typedef lent_pointer_wrapper<detail::linked_list_node<T>> linked_node_ptr;
	typedef lent_pointer_wrapper<const detail::linked_list_node<T>> linked_node_const_ptr;
	typedef shared_reference_wrapper<detail::linked_list_node<T>> linked_node_shared_ref;
	typedef shared_reference_wrapper<const detail::linked_list_node<T>> linked_node_const_shared_ref;
	typedef shared_pointer_wrapper<detail::linked_list_node<T>> linked_node_shared_ptr;
	typedef shared_pointer_wrapper<const detail::linked_list_node<T>> linked_node_const_shared_ptr;

public:
	typedef T value_type;
	typedef typename std::add_lvalue_reference<value_type>::type reference;
	typedef typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type const_reference;
	typedef typename std::add_rvalue_reference<value_type>::type rreference;
	typedef typename std::add_const<rreference>::type const_rreference;
	typedef typename std::add_pointer<value_type>::type pointer;
	typedef typename std::add_pointer<typename std::add_const<value_type>::type>::type const_pointer;

	template<typename ... Args>
	linked_list_node(Args&& ... i_args)
	: m_content(std::forward<Args>(i_args) ...)
	{
	}
	~linked_list_node()
	{
	}
	linked_node_ptr get_prev_node()
	{
		return lend(m_prevNode);
	}
	linked_node_const_ptr get_prev_node() const
	{
		return lend(m_prevNode);
	}
	linked_node_ptr get_next_node()
	{
		return lend(m_nextNode);
	}
	linked_node_const_ptr get_next_node() const
	{
		return lend(m_nextNode);
	}
	linked_node_shared_ptr set_prev_node(linked_node_shared_ref i_node)
	{
		if(i_node.get() != this)
		{
			m_prevNode = i_node;
			m_prevNode->m_nextNode = this->ref_from_this();
		}
		else
		{
			DDK_FAIL("Self references are not allowed");
		}

		return m_prevNode;
	}
	linked_node_shared_ptr set_next_node(linked_node_shared_ref i_node)
	{
		if(i_node.get() != this)
		{
			m_nextNode = i_node;
			i_node->m_prevNode = this->ref_from_this();
		}
		else
		{
			DDK_FAIL("Self references are not allowed");
		}

		return m_nextNode;
	}
	linked_node_shared_ptr extract_prev_node()
	{
		if(m_prevNode)
		{
			m_prevNode->m_nextNode = nullptr;
		}

		return std::move(m_prevNode);
	}
	linked_node_shared_ptr extract_next_node()
	{
		if(m_nextNode)
		{
			m_nextNode->m_prevNode = nullptr;
		}

		return std::move(m_nextNode);
	}
	static linked_node_ptr collapse(linked_node_ptr i_node)
	{
		linked_node_ptr res = lend(i_node->m_nextNode);
		linked_node_shared_ptr prevNode = i_node->m_prevNode;
		linked_node_shared_ptr nextNode = i_node->m_nextNode;

		i_node->m_prevNode = nullptr;
		i_node->m_nextNode = nullptr;
		i_node = nullptr;

		if(prevNode)
		{
			prevNode->m_nextNode = nextNode;
		}

		if(nextNode)
		{
			nextNode->m_prevNode = prevNode;
		}

		return res;
	}
	reference get()
	{
		return m_content;
	}
	const_reference get() const
	{
		return m_content;
	}
	rreference extract() &&
	{
		return std::move(m_content);
	}
	pointer get_ptr()
	{
		return &m_content;
	}
	const_pointer get_ptr() const
	{
		return &m_content;
	}
	bool operator==(const linked_list_node<T>& other) const
	{
		return m_content == other.m_content;
	}
	bool operator!=(const linked_list_node<T>& other) const
	{
		return m_content != other.m_content;
	}

private:
	T m_content;
	linked_node_shared_ptr m_prevNode;
	linked_node_shared_ptr m_nextNode;
};

}

}
