
namespace ddk
{

template<typename T,size_t ... ranks>
iterable_adaptor<high_order_array<T,ranks...>>::iterable_adaptor(high_order_array<T,ranks...>& i_iterable,const ddk::shift_action& i_initialAction)
: m_iterable(i_iterable)
{
	m_currIndex = i_initialAction.shifting();
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<high_order_array<T,ranks...>>::difference_type iterable_adaptor<high_order_array<T,ranks...>>::forward_next_value_in(Sink&& i_sink)
{
	if(m_currIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink.apply(m_iterable.at(++m_currIndex));

		return 0;
	}
	else
	{
		return 1;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<high_order_array<T,ranks...>>::difference_type iterable_adaptor<high_order_array<T,ranks...>>::forward_next_value_in(Sink&& i_sink) const
{
	if(m_currIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink.apply(m_iterable.at(++m_currIndex));

		return 0;
	}
	else
	{
		return 1;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<high_order_array<T,ranks...>>::difference_type iterable_adaptor<high_order_array<T,ranks...>>::forward_prev_value_in(Sink&& i_sink)
{
	if(m_currIndex > 0)
	{
		i_sink.apply(m_iterable.at(--m_currIndex));

		return 0;
	}
	else
	{
		return -1;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<high_order_array<T,ranks...>>::difference_type iterable_adaptor<high_order_array<T,ranks...>>::forward_prev_value_in(Sink&& i_sink) const
{
	if(m_currIndex > 0)
	{
		i_sink.apply(m_iterable.at(--m_currIndex));

		return 0;
	}
	else
	{
		return -1;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<high_order_array<T,ranks...>>::difference_type iterable_adaptor<high_order_array<T,ranks...>>::forward_shift_value_in(difference_type i_shift,Sink&& i_sink)
{
	const difference_type newIndex = m_currIndex + i_shift;

	if(newIndex >= 0 && newIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink.apply(m_iterable.at(m_currIndex = newIndex));

		return 0;
	}
	else
	{
		return (newIndex > 0) ? newIndex - (high_order_array<T,ranks ...>::s_totalSize - 1) : newIndex;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<high_order_array<T,ranks...>>::difference_type iterable_adaptor<high_order_array<T,ranks...>>::forward_shift_value_in(difference_type i_shift,Sink&& i_sink) const
{
	const difference_type newIndex = m_currIndex + i_shift;

	if(newIndex >= 0 && newIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink.apply(m_iterable.at(m_currIndex = newIndex));

		return 0;
	}
	else
	{
		return (newIndex > 0) ? newIndex - (high_order_array<T,ranks ...>::s_totalSize - 1) : newIndex;
	}
}
template<typename T,size_t ... ranks>
bool iterable_adaptor<high_order_array<T,ranks...>>::valid() const noexcept
{
	return 0 <= m_currIndex && m_currIndex < high_order_array<T,ranks ...>::s_totalSize;
}



template<typename T,size_t ... ranks>
iterable_adaptor<const high_order_array<T,ranks...>>::iterable_adaptor(const high_order_array<T,ranks...>& i_iterable,const ddk::shift_action& i_initialAction)
: m_iterable(i_iterable)
{
	m_currIndex = i_initialAction.shifting();
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<const high_order_array<T,ranks...>>::difference_type iterable_adaptor<const high_order_array<T,ranks...>>::forward_next_value_in(Sink&& i_sink)
{
	if(m_currIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink.apply(m_iterable.at(++m_currIndex));

		return 0;
	}
	else
	{
		return 1;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<const high_order_array<T,ranks...>>::difference_type iterable_adaptor<const high_order_array<T,ranks...>>::forward_next_value_in(Sink&& i_sink) const
{
	if(m_currIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink.apply(m_iterable.at(++m_currIndex));

		return 0;
	}
	else
	{
		return 1;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<const high_order_array<T,ranks...>>::difference_type iterable_adaptor<const high_order_array<T,ranks...>>::forward_prev_value_in(Sink&& i_sink)
{
	if(m_currIndex > 0)
	{
		i_sink.apply(m_iterable.at(--m_currIndex));

		return 0;
	}
	else
	{
		return -1;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<const high_order_array<T,ranks...>>::difference_type iterable_adaptor<const high_order_array<T,ranks...>>::forward_prev_value_in(Sink&& i_sink) const
{
	if(m_currIndex > 0)
	{
		i_sink.apply(m_iterable.at(--m_currIndex));

		return 0;
	}
	else
	{
		return -1;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<const high_order_array<T,ranks...>>::difference_type iterable_adaptor<const high_order_array<T,ranks...>>::forward_shift_value_in(difference_type i_shift,Sink&& i_sink)
{
	const difference_type newIndex = m_currIndex + i_shift;

	if(newIndex >= 0 && newIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink.apply(m_iterable.at(m_currIndex = newIndex));

		return 0;
	}
	else
	{
		return (newIndex > 0) ? newIndex - (high_order_array<T,ranks ...>::s_totalSize - 1) : newIndex;
	}
}
template<typename T,size_t ... ranks>
template<typename Sink>
typename iterable_adaptor<const high_order_array<T,ranks...>>::difference_type iterable_adaptor<const high_order_array<T,ranks...>>::forward_shift_value_in(difference_type i_shift,Sink&& i_sink) const
{
	const difference_type newIndex = m_currIndex + i_shift;

	if(newIndex >= 0 && newIndex < high_order_array<T,ranks ...>::s_totalSize)
	{
		i_sink.apply(m_iterable.at(m_currIndex = newIndex));

		return 0;
	}
	else
	{
		return (newIndex > 0) ? newIndex - (high_order_array<T,ranks ...>::s_totalSize - 1) : newIndex;
	}
}
template<typename T,size_t ... ranks>
bool iterable_adaptor<const high_order_array<T,ranks...>>::valid() const noexcept
{
	return 0 <= m_currIndex && m_currIndex < high_order_array<T,ranks ...>::s_totalSize;
}

}
