
#include "ddk_allocator_exceptions.h"
#include "ddk_system_reference_wrapper_allocator.h"

namespace ddk
{

template<typename T, typename Allocator>
linked_list<T,Allocator>::linked_list(const linked_list<T>& other)
{
	const_iterator itNode = other.m_firstNode;
	for(;itNode!=NULL;++itNode)
	{
		if(void* __mem = m_allocator.allocate(1,sizeof(detail::linked_list_node<T>)))
		{
			detail::linked_list_node<T>* newNode = new (__mem) detail::linked_list_node<T>(*itNode);

			_push(as_shared_reference(newNode,get_reference_wrapper_deleter<detail::linked_list_node<T>>(m_allocator)));
		}
		else
		{
			throw bad_allocation_exception{ "Could not allocate linked list node" };
		}
	}
}
template<typename T, typename Allocator>
linked_list<T,Allocator>::linked_list(linked_list<T>&& other)
{
	std::swap(m_firstNode,other.m_firstNode);
	std::swap(m_lastNode,other.m_lastNode);
}
template<typename T, typename Allocator>
linked_list<T,Allocator>::~linked_list()
{
	clear();
}
template<typename T, typename Allocator>
template<typename ... Args>
T& linked_list<T,Allocator>::push(Args&& ... i_args)
{
	if(void* __mem =  m_allocator.allocate(1,sizeof(detail::linked_list_node<T>)))
	{
		detail::linked_list_node<T>* newNode = new (__mem) detail::linked_list_node<T>(std::forward<Args>(i_args) ...);

		return _push(as_shared_reference(newNode,get_reference_wrapper_deleter(m_allocator)));
	}
	else
	{
		throw bad_allocation_exception{"Could not allocate linked list node"};
	}
}
template<typename T, typename Allocator>
optional<T> linked_list<T,Allocator>::pop()
{
	if(linked_node_ptr firstNode = lend(m_firstNode))
	{
		T res = firstNode->extract_value();

		m_firstNode = firstNode->extract_next_node();

		return std::move(res);
	}
	else
	{
		return none;
	}
}
template<typename T, typename Allocator>
void linked_list<T,Allocator>::clear()
{
	linked_node_shared_ptr nextNode = m_firstNode;

	while(nextNode)
	{
		nextNode = nextNode->extract_next_node();
	}

	m_firstNode = nullptr;
	m_lastNode = nullptr;
}
template<typename T, typename Allocator>
typename linked_list<T,Allocator>::iterator linked_list<T,Allocator>::begin()
{
	return iterator(lend(m_firstNode));
}
template<typename T, typename Allocator>
typename linked_list<T,Allocator>::const_iterator linked_list<T,Allocator>::begin() const
{
	return const_iterator(lend(m_firstNode));
}
template<typename T, typename Allocator>
typename linked_list<T,Allocator>::iterator linked_list<T,Allocator>::end()
{
	return nullptr;
}
template<typename T, typename Allocator>
typename linked_list<T,Allocator>::const_iterator linked_list<T,Allocator>::end() const
{
	return nullptr;
}
template<typename T, typename Allocator>
typename linked_list<T,Allocator>::iterator linked_list<T,Allocator>::erase(iterator i_it)
{
	ENSURE_SCOPE_LIFETIME(m_firstNode)
	ENSURE_SCOPE_LIFETIME(m_lastNode)

	linked_node_ptr nextNode = nullptr;

	if(linked_node_ptr currNode = i_it.extract())
	{
		if(m_firstNode == currNode)
		{
			m_firstNode = currNode->extract_next_node();
		}
		if(m_lastNode == currNode)
		{
			m_lastNode = currNode->extract_prev_node();
		}

		nextNode = detail::linked_list_node<T>::collapse(std::move(currNode));
	}

	return iterator(nextNode);
}
template<typename T, typename Allocator>
size_t linked_list<T,Allocator>::size() const
{
	size_t res = 0;

	if(linked_node_ptr currNode = m_firstNode)
	{
		do
		{
			++res;
		}
		while(currNode = currNode->get_next_node());
	}

	return res;
}
template<typename T, typename Allocator>
bool linked_list<T,Allocator>::empty() const
{
	return m_firstNode == nullptr;
}
template<typename T, typename Allocator>
T& linked_list<T,Allocator>::_push(linked_node_shared_ref i_node)
{
	if(m_firstNode == nullptr)
	{
		m_firstNode = i_node;
		m_lastNode = m_firstNode;
	}
	else
	{
		m_lastNode = m_lastNode->set_next_node(i_node);
	}

	return m_lastNode->get();
}

}