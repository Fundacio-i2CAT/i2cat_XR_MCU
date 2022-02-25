
namespace ddk
{
namespace detail
{

template<typename Iterable>
template<typename Action>
iterable_adaptor_base<Iterable>::iterable_adaptor_base(Iterable& i_iterable, Action&& i_initialAction)
: m_iterable(i_iterable)
, m_endIterator(std::end(i_iterable))
{
	const size_t iterableSize = m_iterable.size();

	if(iterableSize > 0)
	{
		const difference_type targetShift = i_initialAction.shifting();
		const difference_type shifting = (targetShift < 0) ? static_cast<int>(iterableSize) + targetShift : targetShift;

		m_currIterator = std::next(std::begin(m_iterable),shifting);
	}
	else
	{
		m_currIterator = m_endIterator;
	}
}
template<typename Iterable>
bool iterable_adaptor_base<Iterable>::valid() const noexcept
{
	return m_currIterator != m_endIterator;
}
template<typename Iterable>
template<typename Sink>
bool iterable_adaptor_base<Iterable>::forward_add_value_in(const_reference i_value, Sink&& i_sink)
{
	typedef typename Iterable::value_type value_type;

	iterator itNew = m_iterable.insert(m_currIterator,i_value);

	if (itNew != this->m_endIterator)
	{
		i_sink.apply(*itNew);

		return true;
	}
	else
	{
		return false;
	}
}
template<typename Iterable>
template<typename Sink>
bool iterable_adaptor_base<Iterable>::forward_erase_value_in(Sink&& i_sink)
{
	iterator itNew = m_iterable.erase(m_currIterator);

	if(itNew != this->m_endIterator)
	{
		i_sink.apply(*itNew);

		return true;
	}
	else
	{
		return false;
	}
}
template<typename Iterable>
template<typename Action>
iterable_adaptor_base<const Iterable>::iterable_adaptor_base(const Iterable& i_iterable,Action&& i_initialAction)
: m_iterable(i_iterable)
, m_endIterator(std::end(i_iterable))
{
	const size_t iterableSize = m_iterable.size();

	if(iterableSize > 0)
	{
		const difference_type targetShift = i_initialAction.shifting();
		const difference_type shifting = (targetShift < 0) ? static_cast<int>(iterableSize) + targetShift : targetShift;

		m_currIterator = std::next(std::begin(m_iterable),shifting);
	}
	else
	{
		m_currIterator = m_endIterator;
	}
}
template<typename Iterable>
bool iterable_adaptor_base<const Iterable>::valid() const noexcept
{
	return m_currIterator != m_endIterator;
}

template<typename Iterable>
template<typename Sink>
typename forward_iterable_adaptor<Iterable>::difference_type forward_iterable_adaptor<Iterable>::forward_next_value_in(Sink&& i_sink)
{
	if(++(this->m_currIterator) != this->m_endIterator)
	{
		i_sink.apply(*(this->m_currIterator));

		return 0;
	}
	else
	{
		return 1;
	}
}
template<typename Iterable>
template<typename Sink>
typename forward_iterable_adaptor<Iterable>::difference_type forward_iterable_adaptor<Iterable>::forward_next_value_in(Sink&& i_sink) const
{
	if(++(this->m_currIterator) != this->m_endIterator)
	{
		i_sink.apply(*(this->m_currIterator));

		return 0;
	}
	else
	{
		return 1;
	}
}

template<typename Iterable>
template<typename Action>
bidirectional_iterable_adaptor<Iterable>::bidirectional_iterable_adaptor(Iterable& i_iterable,Action&& i_initialAction)
: forward_iterable_adaptor<Iterable>(i_iterable,i_initialAction)
, m_endReverseIterator(std::rend(i_iterable))
{
}
template<typename Iterable>
template<typename Sink>
typename bidirectional_iterable_adaptor<Iterable>::difference_type bidirectional_iterable_adaptor<Iterable>::forward_prev_value_in(Sink&& i_sink)
{
	if(std::make_reverse_iterator((this->m_currIterator)--) != m_endReverseIterator)
	{
		i_sink.apply(*(this->m_currIterator));

		return 0;
	}
	else
	{
		return -1;
	}
}
template<typename Iterable>
template<typename Sink>
typename bidirectional_iterable_adaptor<Iterable>::difference_type bidirectional_iterable_adaptor<Iterable>::forward_prev_value_in(Sink&& i_sink) const
{
	if(std::make_reverse_iterator((this->m_currIterator)--) != m_endReverseIterator)
	{
		i_sink.apply(*(this->m_currIterator));

		return 0;
	}
	else
	{
		return -1;
	}
}

template<typename Iterable>
template<typename Sink>
typename random_access_iterable_adaptor<Iterable>::difference_type random_access_iterable_adaptor<Iterable>::forward_shift_value_in(difference_type i_shift, Sink&& i_sink)
{
	switch(i_shift)
	{
	case 1:
		if(++(this->m_currIterator) != this->m_endIterator)
		{
			i_sink.apply(*(this->m_currIterator));

			return 0;
		}
		else
		{
			return 1;
		}
	case -1:
		if(std::make_reverse_iterator((this->m_currIterator)--) != this->m_endReverseIterator)
		{
			i_sink.apply(*(this->m_currIterator));

			return 0;
		}
		else
		{
			return -1;
		}
	default:
		if((this->m_currIterator += i_shift) != this->m_endIterator)
		{
			i_sink.apply(*(this->m_currIterator));

			return 0;
		}
		else
		{
			return i_shift - (std::distance(this->m_endIterator,this->m_currIterator) - 1);
		}
	}
}
template<typename Iterable>
template<typename Sink>
typename random_access_iterable_adaptor<Iterable>::difference_type random_access_iterable_adaptor<Iterable>::forward_shift_value_in(difference_type i_shift, Sink&& i_sink) const
{
	switch(i_shift)
	{
	case 1:
		if(++(this->m_currIterator) != this->m_endIterator)
		{
			i_sink.apply(*(this->m_currIterator));

			return true;
		}
		else
		{
			return 1;
		}
	case -1:
		if(std::make_reverse_iterator((this->m_currIterator)--) != this->m_endReverseIterator)
		{
			i_sink.apply(*(this->m_currIterator));

			return true;
		}
		else
		{
			return -1;
		}
	default:
		if((this->m_currIterator += i_shift) != this->m_endIterator)
		{
			i_sink.apply(*(this->m_currIterator));

			return true;
		}
		else
		{
			return i_shift - (std::distance(this->m_endIterator,this->m_currIterator) - 1);
		}
	}

	return false;
}

}
}
