
namespace ddk
{
namespace detail
{

template<typename ... Iterables>
union_iterable<Iterables...>::union_iterable(shared_reference_wrapper<union_iterable_impl<Iterables...>> i_iterableImpl)
: m_iterableImpl(i_iterableImpl)
{
}
template<typename ... Iterables>
template<typename Iterable>
union_iterable<Iterables...>::operator Iterable() const
{
    return Iterable(m_iterableImpl);
}

}
}
