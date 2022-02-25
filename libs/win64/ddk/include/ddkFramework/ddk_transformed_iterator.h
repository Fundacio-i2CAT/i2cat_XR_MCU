#pragma once

#include <functional>

namespace ddk
{
namespace detail
{

template<typename Iterator, typename ReturnType, typename IteratorTag>
class PrivateIteratorWithoutTransform : public PrivateIterator<Iterator,ReturnType,IteratorTag>
{
	typedef typename Iterator::value_type value_type;
	typedef typename std::add_lvalue_reference<value_type>::type reference;
	typedef typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type const_reference;

public:
	PrivateIteratorWithoutTransform(Iterator i_iterator)
	: PrivateIterator<Iterator,ReturnType,IteratorTag>(i_iterator)
	{}

private:
	virtual ReturnType getValue() const override
	{
	  return *(this->m_privateIterator);
	}
	IIterator<ReturnType,IteratorTag>* clone() const
	{
		return new PrivateIteratorWithoutTransform(this->m_privateIterator);
	}
};

template<typename Iterator, typename ReturnType, typename IteratorTag>
class PrivateIteratorWithTransform : public PrivateIterator<Iterator,ReturnType,IteratorTag>
{
	typedef typename Iterator::value_type value_type;
	typedef typename std::add_lvalue_reference<value_type>::type reference;
	typedef typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type const_reference;

public:
	PrivateIteratorWithTransform(Iterator i_iterator, const ddk::function<ReturnType(const_reference)>& i_transform)
	: PrivateIterator<Iterator,ReturnType,IteratorTag>(i_iterator)
	, m_transform(i_transform)
	{}

private:
	virtual ReturnType getValue() const override
	{
	  return eval(m_transform,*(this->m_privateIterator));
	}
	IIterator<ReturnType,IteratorTag>* clone() const
	{
		return new PrivateIteratorWithTransform(this->m_privateIterator,m_transform);
	}

	ddk::function<ReturnType(const_reference)> m_transform;
};

}
}
