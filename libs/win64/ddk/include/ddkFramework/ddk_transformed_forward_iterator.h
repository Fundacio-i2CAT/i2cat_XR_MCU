#pragma once

#include "ddk_optional.h"
#include "ddk_transformed_iterator_base.h"

namespace ddk
{

template<typename>
class TransformedForwardIterator;

namespace detail
{

//up to forward iterator
template<>
class IIteratorBase<std::forward_iterator_tag>
{
public:
	virtual ~IIteratorBase(){}

	virtual bool equal(const IIteratorBase* other) const = 0;
	virtual const IIteratorBase* get_iterator() const = 0;
	virtual void forward() = 0;
};

template<typename Iterator, typename ReturnType>
class PrivateIterator<Iterator,ReturnType,std::forward_iterator_tag> : public IIterator<ReturnType,std::forward_iterator_tag>, public IIterator_private<Iterator>
{
	typedef std::forward_iterator_tag iterator_tag;
	typedef typename Iterator::reference reference;
	template<typename>
	friend class ddk::TransformedForwardIterator;

protected:
	PrivateIterator(Iterator i_iterator)
	: m_privateIterator(i_iterator)
	{}
	virtual bool equal(const IIteratorBase<std::forward_iterator_tag>* other) const override
	{
		if(const IIterator_private<Iterator>* privateOther = dynamic_cast<const IIterator_private<Iterator>*>(other))
		{
			return m_privateIterator == privateOther->get_private_iterator();
		}
		else
		{
			DDK_FAIL("Comparing two different kind of iterators");

			return false;
		}
	}
	virtual const IIteratorBase<std::forward_iterator_tag>* get_iterator() const override
	{
		return this;
	}
	virtual void forward() override
	{
		++m_privateIterator;
	}
	virtual Iterator& get_private_iterator() override
	{
		return m_privateIterator;
	}
	virtual const Iterator& get_private_iterator() const override
	{
		return m_privateIterator;
	}

	Iterator m_privateIterator;
};

}
}

#include "ddk_transformed_iterator.h"

namespace ddk
{

template<typename Return>
class TransformedForwardIterator
{
	template<typename>
	friend class TransformedForwardIterator;
	template<typename,typename,typename>
	friend class detail::PrivateIterator;

	TransformedForwardIterator(detail::IIterator<Return,std::forward_iterator_tag>* i_iteratorInterface)
	: m_iteratorInterface(i_iteratorInterface)
	{}
public:
	typedef Return reference;
	typedef typename std::add_pointer<Return>::type pointer;
	typedef size_t distance_type;
	typedef std::ptrdiff_t difference_type;
	typedef Return value_type;
	typedef std::bidirectional_iterator_tag iterator_category;

	TransformedForwardIterator()
	: m_iteratorInterface(NULL)
	{}
	TransformedForwardIterator(const TransformedForwardIterator& other)
	: m_iteratorInterface(NULL)
	{
		if(other.m_iteratorInterface)
		{
			m_iteratorInterface = other.m_iteratorInterface->clone();
		}
	}
	template<typename Iterator>
	TransformedForwardIterator(Iterator i_privateIterator)
	{
		m_iteratorInterface = new detail::PrivateIteratorWithoutTransform<Iterator,Return,std::forward_iterator_tag>(i_privateIterator);
	}
	template<typename Iterator, typename Callable>
	TransformedForwardIterator(Iterator i_privateIterator, const Callable& i_transform)
	{
		m_iteratorInterface = new detail::PrivateIteratorWithTransform<Iterator,Return,std::forward_iterator_tag>(i_privateIterator,i_transform);
	}
	~TransformedForwardIterator()
	{
		if(m_iteratorInterface)
		{
			delete m_iteratorInterface;
		}
	}
	TransformedForwardIterator& operator=(const TransformedForwardIterator& other)
	{
		if(m_iteratorInterface)
		{
			delete m_iteratorInterface;
			m_iteratorInterface = NULL;
		}

		if(other.m_iteratorInterface)
		{
			m_iteratorInterface = other.m_iteratorInterface->clone();
		}

		return *this;
	}
	//input iterator
	Return operator*()
	{
		return m_iteratorInterface->getValue();
	}
	Return operator*() const
	{
		return m_iteratorInterface->getValue();
	}
	Return operator->()
	{
		return m_iteratorInterface->getValue();
	}
	Return operator->() const
	{
		return m_iteratorInterface->getValue();
	}
	template<typename RReturn>
	bool operator==(const ddk::TransformedForwardIterator<RReturn>& other) const
	{
		if(m_iteratorInterface && other.m_iteratorInterface)
		{
			return m_iteratorInterface->equal(other.m_iteratorInterface->get_iterator());
		}
		else
		{
			return static_cast<void*>(m_iteratorInterface) == static_cast<void*>(other.m_iteratorInterface);
		}
	}
	template<typename RReturn>
	bool operator!=(const ddk::TransformedForwardIterator<RReturn>& other) const
	{
		if(m_iteratorInterface && other.m_iteratorInterface)
		{
			return m_iteratorInterface->equal(other.m_iteratorInterface->get_iterator()) == false;
		}
		else
		{
			return static_cast<void*>(m_iteratorInterface) != static_cast<void*>(other.m_iteratorInterface);
		}
	}
	//forward iterator
	TransformedForwardIterator& operator++()
	{
		m_iteratorInterface->forward();

		return *this;
	}
	TransformedForwardIterator operator++(int)
	{
		TransformedForwardIterator res = m_iteratorInterface->clone();

		m_iteratorInterface->forward();

		return res;
	}
	template<typename Iterator>
	optional<Iterator> cast_to()
	{
		if (detail::PrivateIterator<Iterator,Return,std::forward_iterator_tag>* privateIterator = dynamic_cast<detail::PrivateIterator<Iterator,Return,std::forward_iterator_tag>*>(m_iteratorInterface))
		{
			return privateIterator->m_privateIterator;
		}
		else
		{
			return none;
		}
	}
	template<typename Iterator>
	optional<Iterator> cast_to() const
	{
		if (const detail::PrivateIterator<Iterator,Return,std::forward_iterator_tag>* privateIterator = dynamic_cast<const detail::PrivateIterator<Iterator,Return,std::forward_iterator_tag>*>(m_iteratorInterface))
		{
			return privateIterator->m_privateIterator;
		}
		else
		{
			return none;
		}
	}

private:
	detail::IIterator<Return,std::forward_iterator_tag>* m_iteratorInterface;
};

namespace detail
{

//JAUME: this specialization is designed for those TransformedBidirectionalIterators whose nested iterators are at the same time TransformedBidirectionalIterators (this way can chain TransformedBidirectionalIterators)
template<typename Iterator,typename ReturnType>
class PrivateIterator<TransformedForwardIterator<Iterator>,ReturnType,std::forward_iterator_tag> : public IIterator<ReturnType,std::forward_iterator_tag>
{
	typedef typename TransformedForwardIterator<Iterator>::reference reference;
	template<typename>
	friend class TransformedForwardIterator;

protected:
	PrivateIterator(const TransformedForwardIterator<Iterator>& i_iterator)
	: m_privateIterator(i_iterator)
	{}
	virtual void forward() override
	{
		++m_privateIterator;
	}
	virtual bool equal(const IIteratorBase<std::forward_iterator_tag>* other) const override
	{
		return m_privateIterator.m_iteratorInterface->equal(other);
	}
	virtual const IIteratorBase<std::forward_iterator_tag>* get_iterator() const override
	{
		return m_privateIterator.m_iteratorInterface->get_iterator();
	}

	TransformedForwardIterator<Iterator> m_privateIterator;
};

}
}
