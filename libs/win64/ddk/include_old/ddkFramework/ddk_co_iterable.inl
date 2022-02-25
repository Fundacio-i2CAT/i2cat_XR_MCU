
namespace ddk
{

template<typename Iterable>
co_iterable<Iterable>::co_iterable(Iterable& i_iterable)
: m_iterable(i_iterable)
{
}
template<typename Iterable>
typename co_iterable<Iterable>::iterator co_iterable<Iterable>::begin()
{
	return iterator(m_iterable);
}
template<typename Iterable>
typename co_iterable<Iterable>::iterator co_iterable<Iterable>::end()
{
	return none;
}
template<typename Iterable>
typename co_iterable<Iterable>::const_iterator co_iterable<Iterable>::begin() const
{
	return const_iterator(m_iterable);
}
template<typename Iterable>
typename co_iterable<Iterable>::const_iterator co_iterable<Iterable>::end() const
{
	return none;
}

template<typename Iterable>
co_iterable<Iterable> co_iterate(Iterable& i_iterable)
{
	return i_iterable;
}
template<typename Iterable>
co_iterable<Iterable> co_reverse_iterate(Iterable& i_iterable)
{
	return i_iterable;
}
template<typename Iterable>
co_iterable<const Iterable> co_iterate(const Iterable& i_iterable)
{
	return i_iterable;
}
template<typename Iterable>
co_iterable<const Iterable> co_reverse_iterate(const Iterable& i_iterable)
{
	return i_iterable;
}

}