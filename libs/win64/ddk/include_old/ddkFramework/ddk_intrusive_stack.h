#pragma once

#include "ddk_intrusive_node.h"

namespace ddk
{

template<typename T>
struct intrusive_stack
{
	typedef lent_reference_wrapper<intrusive_node<T>> intrusive_node_ref;
	typedef lent_reference_wrapper<const intrusive_node<T>> intrusive_node_const_ref;
	typedef lent_pointer_wrapper<intrusive_node<T>> intrusive_node_ptr;
	typedef lent_pointer_wrapper<const intrusive_node<T>> intrusive_node_const_ptr;

public:
	enum Type
	{
		Lifo,
		Fifo
	};

	intrusive_stack(Type i_type)
	: m_firstNode(nullptr)
	, m_lastNode(nullptr)
	, m_type(i_type)
	{
	}
	intrusive_stack(const intrusive_stack<T>& other)
	: m_firstNode(other.m_firstNode)
	, m_lastNode(other.m_lastNode)
	, m_type(other.m_type)
	{
	}
	intrusive_stack(intrusive_stack<T>&& other)
	: m_firstNode(nullptr)
	, m_lastNode(nullptr)
	, m_type(other.m_type)
	{
		std::swap(m_firstNode,other.m_firstNode);
		std::swap(m_lastNode,other.m_lastNode);
	}
	~intrusive_stack()
	{
		clear();
	}
	void push(intrusive_node_ref i_node)
	{
		//DDK_ASSERT(already_contains_node(i_node) == false, "Trying to insert already present node, about to produce a cycle");

		if(m_firstNode == nullptr)
		{
			m_firstNode = std::move(i_node);
			m_lastNode = m_firstNode;
		}
		else if(m_type == Fifo)
		{
			m_lastNode->set_next_node(i_node);
			m_lastNode = i_node;
		}
		else if(m_type == Lifo)
		{
			i_node->set_next_node(m_firstNode);
			m_firstNode = i_node;
		}
	}
	void pop()
	{
		if(m_firstNode)
		{
			if(intrusive_node_ptr firstNode = m_firstNode->extract_next_node())
			{
				m_firstNode = firstNode;
				m_firstNode->clear_prev_node();
			}
			else
			{
				m_firstNode = m_lastNode = nullptr;
			}
		}
	}
	const T& get_front_value() const
	{
		DDK_ASSERT(m_firstNode != nullptr, "Accessing null stack");

		if(m_firstNode)
		{
			return m_firstNode->get_value();
		}
		else
		{
			return crash_on_return<const T&>::value();
		}
	}
	T& get_front_value()
	{
		DDK_ASSERT(m_firstNode != nullptr, "Accessing null stack");

		if(m_firstNode)
		{
			return m_firstNode->get_value();
		}
		else
		{
			return crash_on_return<T&>::value();
		}
	}
	const T& get_back_value() const
	{
		DDK_ASSERT(m_lastNode != nullptr, "Accessing null stack");

		if(m_lastNode)
		{
			return m_lastNode->get_value();
		}
		else
		{
			return crash_on_return<const T&>::value();
		}
	}
	T& get_back_value()
	{
		DDK_ASSERT(m_lastNode != nullptr, "Accessing null stack");

		if(m_lastNode)
		{
			return m_lastNode->get_value();
		}
		else
		{
			return crash_on_return<T&>::value();
		}
	}
	void clear()
	{
		if(m_firstNode)
		{
			m_firstNode->clear();
			m_firstNode = nullptr;
		}
		m_lastNode = nullptr;
	}
	size_t size() const
	{
		size_t res = 0;
		
		if(intrusive_node_ptr currNode = m_firstNode)
		{
			do
			{
				++res;
			}
			while(currNode = currNode->get_next_node());
		}

		return res;
	}
	bool empty() const
	{
		return m_firstNode == nullptr;
	}

private:
	bool already_contains_node(intrusive_node_ptr i_node) const
	{
		if(intrusive_node_ptr currNode = m_firstNode)
		{
			do
			{
				if(currNode == i_node)
				{
					return true;
				}
			}
			while(currNode = currNode->get_next_node());
		}

		return false;
	}

	intrusive_node_ptr m_firstNode;
	intrusive_node_ptr m_lastNode;
	Type m_type;
};

}
