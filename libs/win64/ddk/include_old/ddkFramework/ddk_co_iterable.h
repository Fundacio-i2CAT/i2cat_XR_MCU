#pragma once

#include "ddk_lendable.h"
#include "ddk_template_helper.h"
#include "ddk_co_iterator.h"

namespace ddk
{

template<typename Iterable>
class co_iterable
{
public:
	typedef typename mpl::which_type<std::is_const<Iterable>::value,typename Iterable::const_reference,typename Iterable::reference>::type reference;
	typedef typename Iterable::const_reference const_reference;
	typedef detail::co_iterator_correspondence<Iterable> iterator;
	typedef detail::co_iterator_correspondence<const Iterable> const_iterator;

	friend inline iterator begin(co_iterable& i_co_iterable)
	{
		return i_co_iterable.m_iterable;
	}
	friend inline iterator end(co_iterable&)
	{
		return none;
	}
	friend inline const_iterator begin(const co_iterable& i_co_iterable)
	{
		return i_co_iterable.m_iterable;
	}
	friend inline const_iterator end(const co_iterable&)
	{
		return none;
	}

	co_iterable(Iterable& i_iterable);

	inline iterator begin();
	inline iterator end();
	inline const_iterator begin() const;
	inline const_iterator end() const;

private:
	Iterable& m_iterable;
};

template<typename Iterable>
co_iterable<Iterable> co_iterate(Iterable& i_iterable);
template<typename Iterable>
co_iterable<Iterable> co_reverse_iterate(Iterable& i_iterable);
template<typename Iterable>
co_iterable<const Iterable> co_iterate(const Iterable& i_iterable);
template<typename Iterable>
co_iterable<const Iterable> co_reverse_iterate(const Iterable& i_iterable);

}

#include "ddk_co_iterable.inl"
