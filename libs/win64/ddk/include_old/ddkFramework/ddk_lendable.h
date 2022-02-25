#pragma once

#include "ddk_lent_reference_wrapper.h"
#include "ddk_reference_counter.h"
#include <type_traits>

namespace ddk
{

template<typename T>
class lendable
{
	template<typename TT>
	friend inline lent_reference_wrapper<TT> lend(lendable<TT>&);
	template<typename TT>
	friend inline lent_reference_wrapper<const TT> lend(const lendable<TT>&);
    typedef T value_type;
    typedef typename std::add_const<T>::type const_value_type;
    typedef typename std::add_lvalue_reference<value_type>::type reference;
    typedef typename std::add_lvalue_reference<const_value_type>::type const_reference;
    typedef typename std::add_pointer<value_type>::type pointer_type;
    typedef typename std::add_pointer<const_value_type>::type const_pointer_type;

public:
	lendable()
	{
#ifdef DDK_DEBUG
		m_counter.addStrongReference();
#endif
	}
	template<typename ... Args>
	lendable(Args&& ... i_args)
	: m_value(T(std::forward<Args>(i_args) ...))
	{
#ifdef DDK_DEBUG
		m_counter.addStrongReference();
#endif
	}
	lendable(const lendable& other)
	: m_value(other.m_value)
	{
#ifdef DDK_DEBUG
		m_counter.addStrongReference();
#endif
	}
	lendable(lendable&& other)
	: m_value(std::move(other.m_value))
	{
#ifdef DDK_DEBUG
		m_counter.addStrongReference();
#endif
	}
	~lendable()
	{
#ifdef DDK_DEBUG
		m_counter.removeStrongReference();

		DDK_ASSERT(m_counter.hasLentReferences() == false, "Still lent references alive while destroying unique reference");
#endif
	}
	lendable& operator=(const lendable& other)
	{
		m_value = other.m_value;

		return *this;
	}
	lendable& operator=(lendable&& other)
	{
		m_value = std::move(other.m_value);

		return *this;
	}
	pointer_type operator->()
	{
		return &m_value;
	}
	const_pointer_type operator->() const
	{
		return &m_value;
	}
	reference operator*()
	{
		return m_value;
	}
	const_reference operator*() const
	{
		return m_value;
	}

private:
#ifdef DDK_DEBUG
	inline lent_reference_wrapper<value_type> ref_from_this()
	{
		return lent_reference_wrapper<value_type>(&m_value,&m_counter);
	}
	inline lent_reference_wrapper<const_value_type> ref_from_this() const
	{
	  return lent_reference_wrapper<const_value_type>(&m_value,&m_counter);
	}
#else
	inline lent_reference_wrapper<value_type> ref_from_this()
	{
		return lent_reference_wrapper<value_type>(&m_value);
	}
	inline lent_reference_wrapper<const_value_type> ref_from_this() const
	{
		return lent_reference_wrapper<const_value_type>(&m_value);
	}
#endif

	T m_value;
#ifdef DDK_DEBUG
	mutable unique_reference_counter m_counter;
#endif
};

}
