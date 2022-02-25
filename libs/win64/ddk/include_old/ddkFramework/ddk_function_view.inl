
#include "ddk_reference_wrapper.h"

namespace ddk
{

template<typename Return, typename ... Types>
function_view<Return(Types...)>::function_view(std::nullptr_t)
: m_functionImpl(nullptr)
{
}
template<typename Return, typename ... Types>
function_view<Return(Types...)>::function_view(function_base_const_lent_ptr i_function)
: m_functionImpl(i_function)
{
}
template<typename Return, typename ... Types>
template<typename ... Args>
Return function_view<Return(Types...)>::operator()(Args&& ... i_args) const
{
	if (m_functionImpl)
	{
		return m_functionImpl->operator()(std::forward<Args>(i_args) ...);
	}
	else
	{
		throw std::exception{};
	}
}
template<typename Return, typename ... Types>
function_view<Return(Types...)>::operator bool() const
{
	return m_functionImpl != nullptr;
}
template<typename Return, typename ... Types>
bool function_view<Return(Types...)>::operator==(std::nullptr_t) const
{
	return m_functionImpl == nullptr;
}

}
