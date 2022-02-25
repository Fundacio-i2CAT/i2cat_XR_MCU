#pragma once

#include <iterator>
#include "ddk_lent_pointer_wrapper.h"

namespace ddk
{

template<typename>
struct list_iterator;

template<typename T>
struct list_iterator
{
    static_assert(sizeof(T) == 0, "Invalid type for list iterator");
};

template<typename T>
struct list_iterator<lent_pointer_wrapper<T>>
{
	template<typename>
	friend struct list_iterator;
	template<typename>
	friend struct list_const_iterator;

    typedef int difference_type;
	typedef typename T::value_type value_type;
	typedef typename mpl::static_if<std::is_const<T>::value,typename T::const_reference, typename T::reference>::type reference;
	typedef typename T::const_reference const_reference;
	typedef typename mpl::static_if<std::is_const<T>::value,typename T::const_pointer,typename T::pointer>::type pointer;
	typedef typename T::const_pointer const_pointer;
	typedef std::forward_iterator_tag iterator_category;

	list_iterator(const std::nullptr_t&)
	: m_currNode(nullptr)
	{
	}
	explicit list_iterator(lent_pointer_wrapper<T> i_node)
	: m_currNode(i_node)
	{
	}
	list_iterator(const list_iterator<lent_pointer_wrapper<T>>& other)
	: m_currNode(other.m_currNode)
	{
	}
	list_iterator(list_iterator<lent_pointer_wrapper<T>>&& other)
	: m_currNode(std::move(other.m_currNode))
	{
	}
	template<typename TT>
	list_iterator(const list_iterator<lent_pointer_wrapper<TT>>& other)
	: m_currNode(other.m_currNode)
	{
	}
	template<typename TT>
	list_iterator(list_iterator<lent_pointer_wrapper<TT>>&& other)
	: m_currNode(std::move(other.m_currNode))
	{
	}
	list_iterator& operator=(const list_iterator<lent_pointer_wrapper<T>>& other)
	{
		m_currNode = other.m_currNode;

		return *this;
	}
	list_iterator& operator=(list_iterator<lent_pointer_wrapper<T>>&& other)
	{
		m_currNode = std::move(other.m_currNode);

		return *this;
	}
	template<typename TT>
	list_iterator& operator=(const list_iterator<lent_pointer_wrapper<TT>>& other)
	{
		m_currNode = other.m_currNode;

		return *this;
	}
	template<typename TT>
	list_iterator& operator=(list_iterator<lent_pointer_wrapper<TT>>&& other)
	{
		m_currNode = std::move(other.m_currNode);

		return *this;
	}
	list_iterator operator++(int)
	{
		list_iterator res(m_currNode);

		m_currNode = m_currNode->get_next_node();

		return res;
	}
	list_iterator& operator++()
	{
		m_currNode = m_currNode->get_next_node();

		return *this;
	}
	inline reference operator*()
	{
		return m_currNode->get();
	}
	inline const_reference operator*() const
	{
		return m_currNode->get();
	}
	inline pointer operator->()
	{
		return m_currNode->get_ptr();
	}
	inline const_pointer operator->() const
	{
		return m_currNode->get_ptr();
	}
	inline bool operator==(const list_iterator<lent_pointer_wrapper<T>>& other) const
	{
		return m_currNode == other.m_currNode;
	}
	inline bool operator!=(const list_iterator<lent_pointer_wrapper<T>>& other) const
	{
		return m_currNode != other.m_currNode;
	}
	lent_pointer_wrapper<const T> get() const
	{
		return m_currNode;
	}
	lent_pointer_wrapper<T> get()
	{
		return *m_currNode;
	}
	lent_pointer_wrapper<T> extract()
	{
		return std::move(m_currNode);
	}
	void reset()
	{
		m_currNode = nullptr;
	}

private:
	lent_pointer_wrapper<T> m_currNode;
};

}
