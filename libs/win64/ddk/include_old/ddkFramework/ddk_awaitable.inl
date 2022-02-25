
#include "ddk_sync_executor.h"

namespace ddk
{

template<typename T>
awaited_result<T>::awaited_result(const detail::none_t&)
{
}
template<typename T>
template<typename Arg>
awaited_result<T>::awaited_result(Arg&& i_arg)
{
	m_content.template construct<result_type>(std::forward<Arg>(i_arg));
}
template<typename T>
awaited_result<T>::awaited_result(awaited_result&& other)
{
	if(other.m_content.empty() == false)
	{
		m_content.template construct<result_type>(other.m_content.template extract<result_type>());
	}
}
template<typename T>
awaited_result<T>::~awaited_result()
{
	m_content.template destroy<result_type>();
}
template<typename T>
awaited_result<T>& awaited_result<T>::operator=(awaited_result<T>&& other)
{
	if(other.m_content.empty() == false)
	{
		if constexpr (std::is_copy_assignable<result_type>::value)
		{
			m_content.template assign<result_type>(other.m_content.template extract<result_type>());
		}
		else
		{
			m_content.template destroy<result_type>();

			m_content.template construct<result_type>(other.m_content.template extract<result_type>());
		}
	}
	else if(m_content.empty() == false)
	{
		m_content.template destroy<result_type>();
	}

	return *this;
}
template<typename T>
typename awaited_result<T>::const_reference awaited_result<T>::get() const
{
	if(m_content.empty())
	{
		throw async_exception{ "Trying to access empty result." };
	}
	else
	{
		const result_type& res = m_content.template get<result_type>();

		if(res.template is<T>())
		{
			return res.template get<T>();
		}
		else
		{
			throw res.template get<async_exception>();
		}
	}
}
template<typename T>
typename awaited_result<T>::reference awaited_result<T>::get()
{
	if(m_content.empty())
	{
		throw async_exception{ "Trying to access empty result." };
	}
	else
	{
		result_type& res = m_content.template get<result_type>();

		if(res.template is<T>())
		{
			return res.template get<T>();
		}
		else
		{
			throw res.template get<async_exception>();
		}
	}
}
template<typename T>
awaited_result<T>::operator bool() const
{
	if(m_content.empty() == false)
	{
		const result_type& res = m_content.template get<result_type>();

		return res.template is<T>();
	}
	else
	{
		return false;
	}
}
template<typename T>
void awaited_result<T>::set(result_reference i_content)
{
	if(i_content.template is<reference>())
	{
		m_content.template construct<result_type>(std::forward<reference>(i_content.template get<reference>()));
	}
	else
	{
		m_content.template construct<result_type>(i_content.template get<async_exception>());
	}
}

template<typename T, typename Result>
awaitable<T,Result>::awaitable(const function<T()>& i_function, const detail::this_thread_t& i_thread)
: m_executor(i_function)
{
}
template<typename T, typename Result>
awaitable<T,Result>::awaitable(const awaitable& other)
: m_executor(other.m_executor)
{
}
template<typename T, typename Result>
awaitable<T,Result>::awaitable(awaitable&& other)
: m_executor(std::move(other.m_executor))
{
}
template<typename T, typename Result>
Result awaitable<T,Result>::resume()
{
	Result res;

	m_executor.resume(make_function(&res,&Result::set));

	return res;
}

template<typename Result>
awaitable<void,Result>::awaitable(const ddk::function<void()>& i_function, const detail::this_thread_t& i_thread)
: awaitable<detail::void_t,Result>(make_function([i_function]() { eval(i_function); return _void; }))
{
}

}
