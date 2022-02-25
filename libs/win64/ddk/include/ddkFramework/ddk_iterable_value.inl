
namespace ddk
{

template<typename Traits>
iterable_value<Traits>::iterable_value(reference i_value, const function<reference(action)>& i_resolver, detail::iterable_interface& i_iterableInterface)
: m_value(Traits::get_address(i_value))
, m_resolver(i_resolver)
, m_iterableInterface(i_iterableInterface)
{
}
template<typename Traits>
template<typename Reference, typename Return, typename Action>
iterable_value<Traits>::iterable_value(Reference&& i_value, const function<Return(Action)>& i_resolver, detail::iterable_interface& i_iterableInterface)
: m_value(Traits::get_address(i_value))
, m_resolver(make_function([i_resolver](action i_action) -> reference { return eval(i_resolver,i_action); }))
, m_iterableInterface(i_iterableInterface)
{
}
template<typename Traits>
typename iterable_value<Traits>::reference iterable_value<Traits>::operator*()
{
    return Traits::get_value(m_value);
}
template<typename Traits>
typename iterable_value<Traits>::const_reference iterable_value<Traits>::operator*() const
{
    return Traits::get_value(m_value);
}
template<typename Traits>
typename iterable_value<Traits>::pointer iterable_value<Traits>::operator->()
{
    return m_value;
}
template<typename Traits>
typename iterable_value<Traits>::const_pointer iterable_value<Traits>::operator->() const
{
    return m_value;
}
template<typename Traits>
bool iterable_value<Traits>::operator==(const_reference i_value) const
{
    return Traits::get_value(m_value) == i_value;
}
template<typename Traits>
bool iterable_value<Traits>::operator!=(const_reference i_value) const
{
    return Traits::get_value(m_value) == i_value;
}

template<typename T>
template<typename TT>
const_forwarded_value<T>::const_forwarded_value(const_forwarded_value<TT>&& other)
: iterable_value<detail::forward_iterable_traits<T>>(detail::forward_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}
template<typename T>
template<typename TT>
const_forwarded_value<T>::const_forwarded_value(const_bidirectional_value<TT>&& other)
: iterable_value<detail::forward_iterable_traits<T>>(detail::forward_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}
template<typename T>
template<typename TT>
const_forwarded_value<T>::const_forwarded_value(const_random_accessed_value<TT>&& other)
: iterable_value<detail::forward_iterable_traits<T>>(detail::forward_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}

template<typename T>
template<typename TT>
forwarded_value<T>::forwarded_value(forwarded_value<TT>&& other)
: iterable_value<detail::forward_iterable_traits<T>>(detail::forward_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}
template<typename T>
template<typename TT>
forwarded_value<T>::forwarded_value(bidirectional_value<TT>&& other)
: iterable_value<detail::forward_iterable_traits<T>>(detail::forward_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}
template<typename T>
template<typename TT>
forwarded_value<T>::forwarded_value(const_bidirectional_value<TT>&& other)
: iterable_value<detail::forward_iterable_traits<T>>(detail::forward_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}
template<typename T>
template<typename TT>
forwarded_value<T>::forwarded_value(random_accessed_value<TT>&& other)
: iterable_value<detail::forward_iterable_traits<T>>(detail::forward_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}
template<typename T>
template<typename TT>
forwarded_value<T>::forwarded_value(const_random_accessed_value<TT>&& other)
: iterable_value<detail::forward_iterable_traits<T>>(detail::forward_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}

template<typename T>
template<typename TT>
const_bidirectional_value<T>::const_bidirectional_value(const_bidirectional_value<TT>&& other)
: iterable_value<detail::bidirectional_iterable_traits<T>>(detail::bidirectional_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}
template<typename T>
template<typename TT>
const_bidirectional_value<T>::const_bidirectional_value(const_random_accessed_value<TT>&& other)
: iterable_value<detail::bidirectional_iterable_traits<T>>(detail::bidirectional_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}

template<typename T>
template<typename TT>
bidirectional_value<T>::bidirectional_value(bidirectional_value<TT>&& other)
: iterable_value<detail::bidirectional_iterable_traits<T>>(detail::bidirectional_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}
template<typename T>
template<typename TT>
bidirectional_value<T>::bidirectional_value(random_accessed_value<TT>&& other)
: iterable_value<detail::bidirectional_iterable_traits<T>>(detail::bidirectional_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}
template<typename T>
template<typename TT>
bidirectional_value<T>::bidirectional_value(const_random_accessed_value<TT>&& other)
: iterable_value<detail::bidirectional_iterable_traits<T>>(detail::bidirectional_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}

template<typename T>
template<typename TT>
const_random_accessed_value<T>::const_random_accessed_value(const_random_accessed_value<TT>&& other)
: iterable_value<detail::random_access_iterable_traits<T>>(detail::random_access_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}
template<typename T>
template<typename TT>
const_random_accessed_value<T>::const_random_accessed_value(random_accessed_value<TT>&& other)
: iterable_value<detail::random_access_iterable_traits<T>>(detail::random_access_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}

template<typename T>
template<typename TT>
random_accessed_value<T>::random_accessed_value(random_accessed_value<TT>&& other)
: iterable_value<detail::random_access_iterable_traits<T>>(detail::random_access_iterable_traits<T>::get_value(other.m_value),other.m_resolver,other.m_iterableInterface)
{
}

template<typename Value, typename ... Args>
Value make_iterable_value(Args&& ... i_args)
{
    Value res(std::forward<Args>(i_args) ...);

    return std::move(res);
}

}
