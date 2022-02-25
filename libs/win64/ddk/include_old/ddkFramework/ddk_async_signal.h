#pragma once

#include "ddk_async_message_loop.h"
#include "ddk_async_message_queue.h"
#include "ddk_async_message.h"
#include "ddk_reference_wrapper.h"
#include "ddk_intrusive_node.h"
#include "ddk_thread_executor.h"
#include "ddk_signal_connector.h"
#include "ddk_connection.h"

#ifdef _WIN32

#pragma warning ( push )

#pragma warning (disable:4355)

#endif

namespace ddk
{

const unsigned int k_defaultProcessTimeInMs = 10 * 1000;

template<typename>
class async_signal;

template<typename ... Types>
class async_signal<void(Types...)> : protected detail::signal_connector
{
	typedef signal_functor<void(Types...)> signal_functor_t;
	typedef detail::async_builtin_message<typename std::remove_const<typename std::remove_reference<Types>::type>::type ...> builtn_message_type;

public:
	async_signal(const std::chrono::milliseconds& i_updateTime = std::chrono::milliseconds(k_defaultProcessTimeInMs));
	async_signal(thread_executor_unique_ref i_executor);
    detail::connection_base& connect(const ddk::function<void(Types...)>& i_function) const;
	template<typename MessageType>
    detail::connection_base& connect(const ddk::function<void(Types...)>& i_function, lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_messageQueue) const;
	void disconnect();
	template<typename ... Args>
	void execute(Args&& ... i_args) const;

private:
	void disconnect(const detail::connection_base& i_base) const override final;

	const sender_id m_id;
	mutable async_attachable_message_queue<builtn_message_type> m_msgQueue;
	mutable linked_list<intrusive_node<signal_functor_t>> m_callers;
	mutable async_message_loop<builtn_message_type> m_msgLoop;
};

}

#include "ddk_async_signal.inl"

#ifdef _WIN32

#pragma warning ( pop )

#endif
