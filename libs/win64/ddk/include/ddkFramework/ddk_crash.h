#pragma once

#include <algorithm>

namespace ddk
{

template<typename>
class crash_on_return;

template<typename T>
class crash_on_return
{
public:
	typedef T return_type;
	static inline return_type value()
	{
		static T* _ = NULL;

		*reinterpret_cast<volatile int*>(0xDEAD);

		return *_;
	}
};

template<typename T>
class crash_on_return<T&>
{
public:
	typedef T& return_type;
	static inline return_type value()
	{
		static T* _ = NULL;

		*reinterpret_cast<volatile int*>(0xDEAD);

		return *_;
	}
};

template<typename T>
class crash_on_return<T&&>
{
public:
	typedef T&& return_type;
	static inline return_type value()
	{
		static T* _ = NULL;

		*reinterpret_cast<volatile int*>(0xDEAD);

		return std::move(*_);	
	}
};

}
