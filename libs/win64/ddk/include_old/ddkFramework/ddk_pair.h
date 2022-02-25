#pragma once

#include <type_traits>

namespace ddk
{

template<typename T1, typename T2>
class pair;

template<typename T1, typename T2>
class pair
{
private:
	T1 m_first;
	T2 m_second;
public:
	typedef typename std::add_lvalue_reference<T1>::type T1_lreference;
	typedef typename std::add_lvalue_reference<T2>::type T2_lreference;

	template<typename TT1, typename TT2>
	pair(TT1&& i_arg1, TT2&& i_arg2)
	: m_first(std::forward<TT1>(i_arg1))
	, m_second(std::forward<TT2>(i_arg2))
	, first(m_first)
	, second(m_second)
	{
	}

	T1_lreference first;
	T2_lreference second;
};

template<typename T1, typename T2>
class pair<T1&,T2>
{
private:
	T1& m_first;
	T2 m_second;

public:
	typedef T1& T1_lreference;
	typedef typename std::add_lvalue_reference<T2>::type T2_lreference;

	template<typename TT2>
	pair(T1& i_arg1, TT2&& i_arg2)
	: m_first(i_arg1)
	, m_second(std::forward<TT2>(i_arg2))
	, first(m_first)
	, second(m_second)
	{
	}

	T1_lreference first;
	T2_lreference second;
};

template<typename T1, typename T2>
class pair<T1,T2&>
{
private:
	T1 m_first;
	T2& m_second;

public:
	typedef typename std::add_lvalue_reference<T1>::type T1_lreference;
	typedef T2& T2_lreference;

	template<typename TT1>
	pair(TT1&& i_arg1, T2& i_arg2)
	: m_first(std::forward<TT1>(i_arg1))
	, m_second(i_arg2)
	, first(m_first)
	, second(m_second)
	{
	}

	T1_lreference first;
	T2_lreference second;
};

template<typename T1, typename T2>
class pair<T1&,T2&>
{
private:
	T1& m_first;
	T2& m_second;

public:
	typedef T1& T1_lreference;
	typedef T2& T2_lreference;

	pair(T1& i_arg1, T2& i_arg2)
	: m_first(std::forward<T1>(i_arg1))
	, m_second(std::forward<T2>(i_arg2))
	, first(m_first)
	, second(m_second)
	{
	}

	T1_lreference first;
	T2_lreference second;
};

template<typename T1, typename T2>
pair<T1,T2> make_pair(T1&& i_arg1, T2&& i_arg2)
{
	return pair<T1,T2>(std::forward<T1>(i_arg1),std::forward<T2>(i_arg2));
}

}
