
namespace ddk
{

template<typename T>
template<typename ... Args>
constant_callable<T>::constant_callable(Args&& ... i_args)
: m_value(std::forward<Args>(i_args) ...)
{
}
template<typename T>
template<typename ... Args>
T constant_callable<T>::operator()(Args&& ... i_args) const
{
    return m_value;
}

}
