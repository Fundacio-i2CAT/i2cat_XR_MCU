#pragma once

#include <functional>
#include <iterator>
#include <cstddef>

namespace ddk
{

template<typename Traits>
struct filtered_iterator
{
public:
	typedef typename Traits::iterator iterator;
	typedef typename Traits::reference reference;
	typedef typename Traits::const_reference const_reference;
	typedef typename std::remove_reference<reference>::type value_type;
  typedef std::ptrdiff_t difference_type;
	typedef typename std::add_pointer<value_type>::type pointer;
	typedef std::add_const<pointer> const_pointer;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef ddk::function<bool(const_reference)> predicate;

	filtered_iterator()
	: m_filter(nullptr)
	{
		m_currIterator = lookForFirstForwardAppropiateIterator(m_currIterator);
	}
	filtered_iterator(iterator i_currIt, iterator i_sentinel, predicate i_filter = nullptr)
	: m_filter(i_filter)
	, m_currIterator(i_currIt)
	, m_sentinel(i_sentinel)
	{
		m_currIterator = lookForFirstForwardAppropiateIterator(m_currIterator);
	}
	filtered_iterator(iterator i_currIt)
	: m_filter(nullptr)
	, m_currIterator(i_currIt)
	, m_sentinel(i_currIt)
	{
	}
	filtered_iterator& operator++()
	{
		m_currIterator = lookForFirstForwardAppropiateIterator(++m_currIterator);

		return *this;
	}
	filtered_iterator operator++(int)
	{
		filtered_iterator res(*this);

		m_currIterator = lookForFirstForwardAppropiateIterator(++m_currIterator);

		return res;
	}
	filtered_iterator& operator--()
	{
		m_currIterator = lookForFirstBackwardAppropiateIterator(++m_currIterator);

		return *this;
	}
	filtered_iterator operator--(int)
	{
		filtered_iterator res(*this);

		m_currIterator = lookForFirstBackwardAppropiateIterator(++m_currIterator);

		return res;
	}
	const_reference operator*() const
	{
		return *m_currIterator;
	}
	reference operator*()
	{
		return *m_currIterator;
	}
	const_pointer operator->() const
	{
		return &(*m_currIterator);
	}
	pointer operator->()
	{
		return &(*m_currIterator);
	}
	bool operator==(const filtered_iterator& other) const
	{
		return m_currIterator == other.m_currIterator;
	}
	bool operator!=(const filtered_iterator& other) const
	{
		return m_currIterator != other.m_currIterator;
	}
	bool operator==(const iterator& other) const
	{
		return m_currIterator == other;
	}
	bool operator!=(const iterator& other) const
	{
		return m_currIterator != other;
	}

private:
	iterator lookForFirstForwardAppropiateIterator(iterator i_it)
	{
		while (i_it != m_sentinel && m_filter && m_filter(*i_it) == false)
		{
			++i_it;
		}

		return i_it;
	}
	iterator lookForFirstBackwardAppropiateIterator(iterator i_it)
	{
		while (i_it != m_sentinel && m_filter && m_filter(*i_it) == false)
		{
			--i_it;
		}

		return i_it;
	}

	predicate m_filter;
	iterator m_currIterator;
	iterator m_sentinel;
};

}
