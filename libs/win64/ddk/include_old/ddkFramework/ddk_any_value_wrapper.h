#pragma once

#include "ddk_macros.h"
#include "ddk_template_helper.h"

namespace ddk
{

struct value_wrapper_interface
{
	virtual ~value_wrapper_interface(){};
	virtual value_wrapper_interface* clone(value_wrapper_interface* i_arena = NULL) const = 0;
	virtual void destroy() = 0;
	virtual size_t getSize() const = 0;
};

template<typename T>
struct value_wrapper;

//helper for deciding if it is clonable or not
template<bool Clonable>
struct value_wrapper_cloner;

//for values
template<typename T>
struct value_wrapper : public value_wrapper_interface
{
	template<typename>
	friend struct value_wrapper;

public:
	typedef T raw_type;
	typedef T* pointer_type;
	typedef const raw_type* const_pointer_type;
	typedef raw_type& lreference;
	typedef typename std::add_const<raw_type>::type& const_lreference;
	typedef raw_type&& rreference;
	typedef typename std::add_const<raw_type>::type&& const_rreference;

	template<typename TT>
	value_wrapper(TT&& i_value)
	: m_value(std::forward<TT>(i_value))
	{}
	virtual value_wrapper_interface* clone(value_wrapper_interface* i_arena = NULL) const override
	{
		return value_wrapper_cloner<mpl::is_copy_constructible<T>::value>::clone(m_value,i_arena);
	}
	virtual void destroy() override
	{
		//if anybody access this data after destruction something  very bad will happen
		m_value.~T();
	}
	template<typename TT>
	void setValue(TT&& i_value, value_wrapper_interface* i_arena)
	{
		m_value = std::forward<TT>(i_value);
	}
	lreference getValue()
	{
		return m_value;
	}
	const_lreference getValue() const
	{
		return m_value;
	}
	rreference extractValue()
	{
		return std::move(m_value);
	}
	virtual size_t getSize() const override
	{
		return sizeof(value_wrapper<T>);
	}

private:
	value_wrapper(const value_wrapper&);

	raw_type m_value;
};

template<>
struct value_wrapper_cloner<true>
{
	template<typename T>
	static inline value_wrapper_interface* clone(const T& i_value, value_wrapper_interface* i_arena = NULL)
	{
		if(i_arena)
		{
			return new (i_arena) value_wrapper<T>(i_value);
		}
		else
		{
			return new value_wrapper<T>(i_value);
		}
	}
};

template<>
struct value_wrapper_cloner<false>
{
	template<typename T>
	static inline value_wrapper_interface* clone(const T& i_value, value_wrapper_interface* i_arena = NULL)
	{
		DDK_FAIL("You shoul not reach this point");

		return ddk::crash_on_return<value_wrapper_interface*>::value();
	}
};

//for left references
template<typename T>
struct value_wrapper<T&> : public value_wrapper_interface
{
public:
	typedef T raw_type;
	typedef T* pointer_type;
	typedef typename std::add_const<pointer_type>::type const_pointer_type;
	typedef T& lreference;
	typedef typename std::add_const<lreference>::type const_lreference;
	typedef lreference rreference;
	typedef const_lreference const_rreference;

	value_wrapper(lreference i_value)
	: m_value(i_value)
	{}
	virtual value_wrapper_interface* clone(value_wrapper_interface* i_arena = NULL) const override
	{
		if(i_arena)
		{
			return new (i_arena) value_wrapper<T&>(m_value);
		}
		else
		{
			return new value_wrapper<T&>(m_value);
		}
	}
	virtual void destroy() override
	{
		//nothing to be done for an l-value
	}
	void setValue(lreference i_value, value_wrapper_interface* i_arena)
	{
		static const size_t tSize = sizeof(value_wrapper<T&>);

		if (i_arena->getSize() >= tSize)
		{
			new (i_arena) value_wrapper<T&>(i_value);
		}
		else
		{
			MAKE_IT_CRASH
		}
	}
	lreference getValue()
	{
		return m_value;
	}
	lreference extractValue()
	{
		return m_value;
	}
	const_lreference getValue() const
	{
		return m_value;
	}
	virtual size_t getSize() const override
	{
		return sizeof(value_wrapper<T&>);
	}

private:
	lreference m_value;
};

//for right references
template<typename T>
struct value_wrapper<T&&> : public value_wrapper_interface
{
public:
	typedef T raw_type;
	typedef T* pointer_type;
	typedef typename std::add_const<pointer_type>::type const_pointer_type;
	typedef T&& rreference;
	typedef typename std::add_const<rreference>::type const_rreference;
	typedef rreference lreference;
	typedef const_rreference const_lreference;

	value_wrapper(rreference i_value)
	: m_value(std::move(i_value))
	{}
	virtual value_wrapper_interface* clone(value_wrapper_interface* i_arena = NULL) const override
	{
		if(i_arena)
		{
			return new (i_arena) value_wrapper<T&&>(std::move(m_value));
		}
		else
		{
			return new value_wrapper<T&&>(std::move(m_value));
		}
	}
	virtual void destroy() override
	{
		//nothing to be done for an r-value
	}
	void setValue(rreference i_value, value_wrapper_interface* i_arena)
	{
		static const size_t tSize = sizeof(value_wrapper<T&&>);

		if (i_arena->getSize() >= tSize)
		{
			new (i_arena) value_wrapper<T&&>(std::move(i_value));
		}
		else
		{
			MAKE_IT_CRASH
		}
	}
	rreference getValue()
	{
		return m_value;
	}
	rreference extractValue()
	{
		return m_value;
	}
	const_rreference getValue() const
	{
		return m_value;
	}
	virtual size_t getSize() const override
	{
		return sizeof(value_wrapper<T&&>);
	}

private:
	rreference m_value;
};

template<typename T>
struct null_value_wrapper : public value_wrapper_interface
{
	virtual value_wrapper_interface* clone(value_wrapper_interface* i_arena = NULL) const override
	{
		//do nothing
		return NULL;
	}
	virtual void destroy() override
	{
		//do nothing
	}
	virtual size_t getSize() const override
	{
		return sizeof(value_wrapper<T>);
	}
};

}