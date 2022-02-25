
namespace ddk
{

template<typename T>
any_value::any_value(const T& i_val, typename std::enable_if<std::is_same<T,any_value>::value == false>::type*)
: m_currentAgnosticType(k_invalidType)
{
    static const size_t s_TypeHashCode = getTypeId<T>();

    m_currentAgnosticType = s_TypeHashCode;

    m_arena = make_tagged_pointer<value_wrapper<T>>(0x01,i_val);
}
template<typename T>
bool any_value::isOfType() const
{
    static const size_t s_TypeHashCode = getTypeId<T>();

    return m_currentAgnosticType == s_TypeHashCode;
}
template<typename T, typename TT>
void any_value::setValue(TT&& i_value)
{
    static const size_t s_TypeHashCode = getTypeId<T>();

    if(m_currentAgnosticType == s_TypeHashCode)
    {
        value_wrapper<T>* innerValue = static_cast<value_wrapper<T>*>(m_arena.get_pointer());

        innerValue->setValue(std::forward<TT>(i_value),m_arena.get_pointer());
    }
    else
    {
        const bool fitsNewValue = getArenaSize() >= sizeof(value_wrapper<T>);

        destroy();

        if(m_arena.is_tagged())
        {
            if(fitsNewValue)
            {
                m_arena = tagged_pointer<value_wrapper<T>>(new (m_arena.get_pointer()) value_wrapper<T>(std::forward<TT>(i_value)),0x01);
            }
            else
            {
                m_arena = make_tagged_pointer<value_wrapper<T>>(0x01,std::forward<TT>(i_value));
            }
        }
        else
        {
            m_arena = make_tagged_pointer<value_wrapper<T>>(0x01,std::forward<TT>(i_value));
        }

        m_currentAgnosticType = s_TypeHashCode;
    }
}
template<typename T>
typename value_wrapper<T>::lreference any_value::getValue()
{
    static const size_t s_TypeHashCode = getTypeId<T>();

    if(m_currentAgnosticType == s_TypeHashCode)
    {
        if(value_wrapper<T>* innerValue = static_cast<value_wrapper<T>*>(m_arena.get_pointer()))
        {
            return innerValue->getValue();
        }
        else
        {
            throw bad_access_exception{"acessing void arena"};
        }
    }
    else
    {
        throw bad_access_exception{"accessing empty container"};
    }
}
template<typename T>
typename value_wrapper<T>::const_lreference any_value::getValue() const
{
    static const size_t s_TypeHashCode = getTypeId<T>();

    if(m_currentAgnosticType == s_TypeHashCode)
    {
        if(const value_wrapper<T>* innerValue = static_cast<const value_wrapper<T>*>(m_arena.get_pointer()))
        {
            return innerValue->getValue();
        }
        else
        {
            throw bad_access_exception{"acessing void arena"};
        }
    }
    else
    {
        throw bad_access_exception{"accessing empty container"};
    }
}
template<typename T>
typename value_wrapper<T>::raw_type any_value::extractValue()
{
    static const size_t s_TypeHashCode = getTypeId<T>();

    if(m_currentAgnosticType == s_TypeHashCode)
    {
        if(value_wrapper<T>* innerValue = static_cast<value_wrapper<T>*>(m_arena.get_pointer()))
        {
            typename value_wrapper<T>::raw_type res = innerValue->extractValue();

            destroy();

            return std::move(res);
        }
        else
        {
            throw bad_access_exception{"acessing void arena"};
        }
    }
    else
    {
        throw bad_access_exception{"accessing empty container"};
    }
}

}
