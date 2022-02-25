
namespace ddk
{

template<typename T>
optional<T>::optional()
{
}
template<typename T>
optional<T>::optional(detail::none_t)
{
}
template<typename T>
optional<T>::optional(const optional<T>& other)
{
    if (!other.m_storage.empty())
    {
		m_storage.template construct<T>(other.m_storage.template get<T>());
    }
}
template<typename T>
optional<T>::optional(optional<T>&& other)
{
    if (!other.m_storage.empty())
    {
		m_storage.template construct<T>(other.m_storage.template extract<T>());
    }
}
template<typename T>
optional<T>::optional(reference_const_type val)
{
	m_storage.template construct<T>(val);
}
template<typename T>
optional<T>::optional(reference_type val)
{
	m_storage.template construct<T>(val);
}
template<typename T>
optional<T>::optional(rreference_type val)
{
	m_storage.template construct<T>(std::move(val));
}
template<typename T>
template<typename TT>
optional<T>::optional(const optional<TT>& other)
{
    if (!other.m_storage.empty())
    {
		m_storage.template construct<T>(other.m_storage.template get<TT>());
    }
}
template<typename T>
template<typename TT>
optional<T>::optional(optional<TT>&& other)
{
    if (!other.m_storage.empty())
    {
		m_storage.template construct<T>(other.m_storage.template extract<TT>());
    }
}
template<typename T>
template<typename Arg>
optional<T>::optional(Arg&& i_arg, typename std::enable_if<std::is_convertible<Arg,optional<T>>::value == false>::type*)
{
	m_storage.template construct<T>(std::forward<Arg>(i_arg));
}
template<typename T>
optional<T>::~optional()
{
	m_storage.template destroy<T>();
}
template<typename T>
optional<T>& optional<T>::operator=(const optional<T>& other)
{
    if (other.m_storage.empty() == false)
    {
		m_storage.template set_value<T>(other.m_storage.template get<T>());
    }
    else
    {
		m_storage.template destroy<T>();
    }

    return *this;
}
template<typename T>
optional<T>& optional<T>::operator=(optional<T>&& other)
{
    if (other.m_storage.empty() == false)
    {
		m_storage.template set_value<T>(other.m_storage.template extract<T>());
    }
    else
    {
        m_storage.template destroy<T>();
    }

    return *this;
}
template<typename T>
template<typename TT>
optional<T>& optional<T>::operator=(const optional<TT>& other)
{
    if (other.m_storage.empty() == false)
    {
		m_storage.template set_value<T>(other.m_storage.template extract<TT>());
    }
    else
    {
		m_storage.template destroy<T>();
    }

    return *this;
}
template<typename T>
template<typename TT>
optional<T>& optional<T>::operator=(optional<TT>&& other)
{
    if (other.m_storage.empty() == false)
    {
		m_storage.template set_value<T>(other.m_storage.template extract<TT>());
    }
    else
    {
        m_storage.template destroy<T>();
    }

    return *this;
}
template<typename T>
optional<T>& optional<T>::operator=(detail::none_t)
{
	m_storage.template destroy<T>();

    return *this;
}
template<typename T>
optional<T>& optional<T>::operator=(reference_type val)
{
	m_storage.template set_value<T>(val);

    return *this;
}
template<typename T>
optional<T>& optional<T>::operator=(reference_const_type val)
{
	m_storage.template set_value<T>(val);

    return *this;
}
template<typename T>
optional<T>& optional<T>::operator=(rreference_type val)
{
	m_storage.template set_value<T>(std::move(val));

    return *this;
}
template<typename T>
bool optional<T>::empty() const
{
    return m_storage.empty();
}
template<typename T>
optional<T>::operator bool() const
{
    return m_storage.empty() == false;
}
template<typename T>
bool optional<T>::operator!() const
{
    return m_storage.empty();
}
template<typename T>
typename embedded_type<T>::cref_type optional<T>::get() const
{
	return m_storage.template get<T>();
}
template<typename T>
typename embedded_type<T>::ref_type optional<T>::get()
{
	return m_storage.template get<T>();
}
template<typename T>
typename embedded_type<T>::cref_type optional<T>::get_value_or(typename embedded_type<T>::cref_type default_value) const
{
    if (m_storage.empty() == false)
    {
		return m_storage.template get<T>();
    }

    return default_value;
}
template<typename T>
typename embedded_type<T>::ref_type optional<T>::get_value_or(typename embedded_type<T>::ref_type default_value)
{
    if (m_storage.empty() == false)
    {
		return m_storage.template get<T>();
    }

    return default_value;
}
template<typename T>
typename embedded_type<T>::cpointer_type optional<T>::get_ptr() const
{
    if (m_storage.empty() == false)
    {
		return m_storage.template get<T>();
    }

    return nullptr;
}
template<typename T>
typename embedded_type<T>::pointer_type optional<T>::get_ptr()
{
    if (m_storage.empty() == false)
    {
		return m_storage.template get<T>();
    }

    return nullptr;
}
template<typename T>
T optional<T>::extract()
{
	return m_storage.template extract<T>().extract();
}
template<typename T>
typename embedded_type<T>::cref_type optional<T>::operator*() const
{
	return m_storage.template get<T>();
}
template<typename T>
typename embedded_type<T>::ref_type optional<T>::operator*()
{
	return m_storage.template get<T>();
}
template<typename T>
typename embedded_type<T>::cpointer_type optional<T>::operator->() const
{
	return m_storage.template get_ptr<T>();
}
template<typename T>
typename embedded_type<T>::pointer_type optional<T>::operator->()
{
	return m_storage.template get_ptr<T>();
}
template<typename T>
void optional<T>::swap(const optional<T>& other)
{
    optional<T>& ncOther = const_cast<optional<T>&>(other);

    if (m_storage.empty() == false && ncOther.m_storage.empty() == false)
    {
		std::swap(m_storage.template get<T>(),other.m_storage.template get<T>());
    }
    else if (m_storage.empty() == false)
    {
		other.m_storage.template construct<T>(m_storage.template extract<T>());
    }
    else if (ncOther.m_storage.empty() == false)
    {
		m_storage.template construct<T>(other.m_storage.template extract<T>());
    }
}

}
