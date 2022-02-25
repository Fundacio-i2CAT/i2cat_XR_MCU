#pragma once

#include <tuple>
#include "ddk_variant.h"

namespace ddk
{

template<typename ... Types>
struct tuple_iterator
{
	static const size_t s_numTypes = sizeof...(Types);
	typedef std::tuple<Types...> tuple_t;

public:
	typedef variant<typename std::add_lvalue_reference<Types>::type ...> reference;
	typedef variant<typename std::add_lvalue_reference<typename std::add_const<Types>::type>::type ...> const_reference;

	tuple_iterator(const tuple_t& i_tuple)
	: m_tuple(i_tuple)
	, m_currentIndex(0)
	{
	}
	tuple_iterator(const tuple_t& i_tuple, size_t i_currIndex)
	: m_tuple(i_tuple)
	, m_currentIndex(i_currIndex)
	{
	}
	reference operator*()
	{
		return get(m_currentIndex);
	}
	const_reference operator*() const
	{
		return get(m_currentIndex);
	}
	reference operator->()
	{
		return get(m_currentIndex);
	}
	const_reference operator->() const
	{
		return get(m_currentIndex);
	}
	bool operator==(const tuple_iterator<Types...>& other) const
	{
		return m_currentIndex == other.m_currentIndex;
	}
	bool operator!=(const tuple_iterator<Types...>& other) const
	{
		return m_currentIndex != other.m_currentIndex;
	}
	const tuple_iterator<Types...>& operator++()
	{
		m_currentIndex = (m_currentIndex < s_numTypes) ? m_currentIndex+1 : s_numTypes;

		return *this;
	}
	tuple_iterator<Types...> operator++(int)
	{
		tuple_iterator<Types...> res(m_tuple,m_currentIndex);

		m_currentIndex = (m_currentIndex < s_numTypes) ? m_currentIndex+1 : s_numTypes;

		return res;
	}
	const tuple_iterator<Types...>& operator--()
	{
		m_currentIndex = (m_currentIndex > 0) ? m_currentIndex-1 : s_numTypes;

		return *this;
	}
	tuple_iterator<Types...> operator--(int)
	{
		tuple_iterator<Types...> res(m_tuple,m_currentIndex);

		m_currentIndex = (m_currentIndex > 0) ? m_currentIndex-1 : s_numTypes;

		return res;
	}

private:
	reference get(size_t i_index)
	{
		typedef typename mpl::create_range_rank<0,s_numTypes>::type range_rank;

		return _get(range_rank{},i_index);
	}
	const_reference get(size_t i_index) const
	{
		typedef typename mpl::create_range_rank<0,s_numTypes>::type range_rank;

		return _get(range_rank{},i_index);
	}
	template<size_t ... Seq>
	inline reference _get(const mpl::sequence<Seq...>&, size_t i_index)
	{
		typedef reference(*getter_func_t)(tuple_t&);
		static const getter_func_t getterFuncs[s_numTypes] = { &tuple_iterator<Types...>::__get<Seq> ... };

		return (*getterFuncs[i_index])(m_tuple);
	}
	template<size_t ... Seq>
	inline const_reference _get(const mpl::sequence<Seq...>&, size_t i_index) const
	{
		typedef const_reference(*getter_func_t)(const tuple_t&);
		static const getter_func_t getterFuncs[s_numTypes] = { &tuple_iterator<Types...>::__get<Seq> ... };

		return (*getterFuncs[i_index])(m_tuple);
	}
	template<size_t Seq>
	static inline reference __get(tuple_t& i_tuple)
	{
		return std::get<Seq>(i_tuple);
	}
	template<size_t Seq>
	static inline const_reference __get(const tuple_t& i_tuple)
	{
		return std::get<Seq>(i_tuple);
	}

	const tuple_t& m_tuple;
	mutable size_t m_currentIndex;
};

template<typename ... Types>
struct const_tuple_iterator
{
	static const size_t s_numTypes = sizeof...(Types);
	typedef std::tuple<Types...> tuple_t;

public:
	typedef variant<typename std::add_lvalue_reference<typename std::add_const<Types>::type>::type ...> reference;
	typedef reference const_reference;

	const_tuple_iterator(const tuple_t& i_tuple)
	: m_tuple(i_tuple)
	, m_currentIndex(0)
	{
	}
	const_tuple_iterator(const tuple_t& i_tuple, size_t i_currIndex)
	: m_tuple(i_tuple)
	, m_currentIndex(i_currIndex)
	{
	}
	const_reference operator*() const
	{
		return get(m_currentIndex);
	}
	const_reference operator->() const
	{
		return get(m_currentIndex);
	}
	bool operator==(const const_tuple_iterator<Types...>& other) const
	{
		return m_currentIndex == other.m_currentIndex;
	}
	bool operator!=(const const_tuple_iterator<Types...>& other) const
	{
		return m_currentIndex != other.m_currentIndex;
	}
	const const_tuple_iterator<Types...>& operator++() const
	{
		m_currentIndex = (m_currentIndex < s_numTypes) ? m_currentIndex+1 : s_numTypes;

		return *this;
	}
	const_tuple_iterator<Types...> operator++(int) const
	{
		const_tuple_iterator<Types...> res(m_tuple,m_currentIndex);

		m_currentIndex = (m_currentIndex < s_numTypes) ? m_currentIndex+1 : s_numTypes;

		return res;
	}
	const const_tuple_iterator<Types...>& operator--() const
	{
		m_currentIndex = (m_currentIndex > 0) ? m_currentIndex-1 : s_numTypes;

		return *this;
	}
	const_tuple_iterator<Types...> operator--(int) const
	{
		const_tuple_iterator<Types...> res(m_tuple,m_currentIndex);

		m_currentIndex = (m_currentIndex > 0) ? m_currentIndex-1 : s_numTypes;

		return res;
	}

private:
	const_reference get(size_t i_index) const
	{
		typedef typename mpl::create_range_rank<0,s_numTypes>::type range_rank;

		return _get(range_rank{},i_index);
	}
	template<size_t ... Seq>
	inline const_reference _get(const mpl::sequence<Seq...>&, size_t i_index) const
	{
		typedef const_reference(*getter_func_t)(const tuple_t&);
		static const getter_func_t getterFuncs[s_numTypes] = { &const_tuple_iterator<Types...>::__get<Seq> ... };

		return (*getterFuncs[i_index])(m_tuple);
	}
	template<size_t Seq>
	static inline const_reference __get(const tuple_t& i_tuple)
	{
		return std::get<Seq>(i_tuple);
	}

	const tuple_t& m_tuple;
	mutable size_t m_currentIndex;
};

}
