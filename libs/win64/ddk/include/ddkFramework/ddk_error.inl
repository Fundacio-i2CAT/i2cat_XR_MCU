
namespace ddk
{
namespace detail
{

template<typename Error>
error_impl<Error>::error_impl(const Error& i_error)
: m_error(i_error)
{
}
template<typename Error>
error_impl<Error>::error_impl(const Error& i_error, const std::string& i_errorDesc)
: m_error(i_error)
, m_errorDesc(i_errorDesc)
{
}
template<typename Error>
Error error_impl<Error>::get_error() const
{
    return m_error;
}
template<typename Error>
const std::string& error_impl<Error>::get_description() const
{
    return m_errorDesc;
}
template<typename Error>
bool error_impl<Error>::operator==(const error_impl<Error>& i_error) const
{
    return m_error == i_error.m_error;
}
template<typename Error>
bool error_impl<Error>::operator!=(const error_impl<Error>& i_error) const
{
    return m_error == i_error.m_error;
}

template<typename Error, typename ... NestedErrors>
template<typename NestedError>
error_impl<Error,NestedErrors...>::error_impl(const Error& i_errorCode, NestedError&& i_nestedError)
: error_impl<Error>(i_errorCode)
, m_nestedErrors(i_nestedError)
{
}
template<typename Error, typename ... NestedErrors>
template<typename NestedError>
error_impl<Error,NestedErrors...>::error_impl(const Error& i_errorCode, const std::string& i_errorDesc, NestedError&& i_nestedError)
: error_impl<Error>(i_errorCode,i_errorDesc)
, m_nestedErrors(i_nestedError)
{
}
template<typename Error, typename ... NestedErrors>
template<typename NestedError>
const NestedError& error_impl<Error,NestedErrors...>::get_nested_error() const
{
    return m_nestedErrors.template get<NestedError>();
}
template<typename Error, typename ... NestedErrors>
template<typename Visitor>
typename mpl::remove_qualifiers<Visitor>::return_type error_impl<Error,NestedErrors...>::visit(Visitor&& i_visitor) const
{
    return m_nestedErrors.visit(i_visitor);
}

}

template<typename Result, typename ... Args>
Result make_error(Args&& ... i_args)
{
    return Result(typename Result::error_t(std::forward<Args>(i_args) ...));
}

}
