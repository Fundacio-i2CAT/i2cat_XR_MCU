#pragma once

#include <tuple>
#include "ddk_tuple_iterator.h"

namespace ddk
{

template<typename>
struct tuple_iterable;

template<typename ... Types>
struct tuple_iterable<std::tuple<Types...>>
{
	static const size_t s_numTypes = sizeof...(Types);

public:
	typedef tuple_iterator<Types...> iterator;
	typedef const_tuple_iterator<Types...> const_iterator;
	typedef typename const_iterator::reference reference;
	typedef typename const_iterator::const_reference const_reference;

	tuple_iterable(const std::tuple<Types...>& i_tuple)
	: m_tuple(i_tuple)
	{
	}
	iterator begin()
	{
		return iterator(m_tuple);
	}
	iterator end()
	{
		return iterator(m_tuple,s_numTypes);
	}
	const_iterator begin() const
	{
		return const_iterator(m_tuple);
	}
	const_iterator end() const
	{
		return const_iterator(m_tuple,s_numTypes);
	}
	const_iterator cbegin() const
	{
		return const_iterator(m_tuple);
	}
	const_iterator cend() const
	{
		return const_iterator(m_tuple,s_numTypes);
	}

private:
	const std::tuple<Types...>& m_tuple;
};

}