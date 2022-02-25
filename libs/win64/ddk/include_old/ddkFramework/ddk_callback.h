#pragma once

#include "ddk_signal_functor.h"
#include "ddk_connection.h"
#include "ddk_macros.h"
#include "ddk_linked_list.h"
#include "ddk_signal_connector.h"

namespace ddk
{

template<typename>
class callback;

template<typename Return, typename ... Types>
class callback<Return(Types...)> : protected detail::signal_connector
{
private:
	typedef signal_functor<Return(Types...)> signal_functor_t;

    callback(const callback<Return(Types...)>&);
    callback<Return(Types...)>& operator=(const callback<Return(Types...)>&);

public:
	callback()
	{
	}
	callback(callback<Return(Types...)>&& other)
	: m_recipient(std::move(other.m_recipient))
	{
	}
    ~callback()
	{
		disconnect();
	}
    //non static member functions
    detail::connection_base& connect(const ddk::function<Return(Types...)>& i_function) const
	{
		DDK_ASSERT(m_recipient.empty(), "You shall disconnect before reconnecting");

		return m_recipient.push(i_function,static_cast<const detail::signal_connector&>(*this));
	}
    void disconnect()
	{
		m_recipient.clear();
	}
    template<typename ... Args>
	Return execute(Args&& ... i_args) const
	{
		DDK_ASSERT(m_recipient.empty() == false, "Trying to call null function");

        return (*m_recipient.begin()).execute(std::forward<Args>(i_args) ...);
	}
	bool is_connected() const
	{
		return m_recipient.empty() == false;
	}
	operator bool() const
	{
		return m_recipient.empty() == false;
	}

private:
	void disconnect(const detail::connection_base& i_base) const override final
	{
		m_recipient.clear();
	}

	mutable linked_list<signal_functor_t> m_recipient;
};

//void return case
template<typename ... Types>
class callback<void(Types...)> : protected detail::signal_connector
{
private:
	typedef signal_functor<void(Types...)> signal_functor_t;

    callback(const callback<void(Types...)>&);
    callback<void(Types...)>& operator=(const callback<void(Types...)>&);

public:
	callback()
	{
	}
	callback(callback<void(Types...)>&& other)
	: m_recipient(std::move(other.m_recipient))
	{
	}
    ~callback()
	{
		disconnect();
	}
    detail::connection_base& connect(const ddk::function<void(Types...)>& i_function) const
	{
		static_assert(std::is_base_of<baseT,T>::value,"There is a mismatch in method and object types!");

		DDK_ASSERT(m_recipient.empty(), "You shall disconnect before reconnecting");

		return m_recipient.push(i_function,static_cast<const detail::signal_connector&>(*this));
	}
    void disconnect()
	{
		m_recipient.clear();
	}
    template<typename ... Args>
	void execute(Args&& ... i_args) const
	{
		DDK_ASSERT(m_recipient.empty() == false, "Trying to call null function");

        (*m_recipient.begin()).execute(std::forward<Args>(i_args) ...);
	}
	bool is_connected() const
	{
		return m_recipient.empty() == false;
	}
	operator bool() const
	{
		return m_recipient.empty() == false;
	}

private:
	void disconnect(const detail::connection_base& i_base) const override final
	{
		m_recipient.clear();
	}

	mutable linked_list<signal_functor_t> m_recipient;
};

}
