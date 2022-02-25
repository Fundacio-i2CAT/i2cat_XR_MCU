
namespace ddk
{
namespace detail
{

template<typename ... Iterables>
intersection_iterable<Iterables...>::intersection_iterable(shared_reference_wrapper<intersection_iterable_impl<Iterables...>> i_iterableImpl)
: m_iterableImpl(i_iterableImpl)
{
}
template<typename ... Iterables>
template<typename Iterable>
intersection_iterable<Iterables...>::operator Iterable() const
{
    return Iterable(m_iterableImpl);
}

}
}

