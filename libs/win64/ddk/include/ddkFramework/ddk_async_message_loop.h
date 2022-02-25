#pragma once

#include <list>
#include "ddk_signal_functor.h"
#include "ddk_linked_list.h"
#include "ddk_intrusive_node.h"
#include "ddk_async_defs.h"
#include "ddk_async_message_queue.h"
#include "ddk_async_message_exchange_room.h"
#include "ddk_reference_wrapper.h"
#include "ddk_mutex.h"
#include "ddk_lock_guard.h"

namespace ddk
{

template<typename BuiltInMessageType>
class async_message_loop
{
	typedef detail::async_message_exchange_room<BuiltInMessageType> msg_exchange_room;
	typedef unique_reference_wrapper<msg_exchange_room> msg_exchange_room_unique_ref;
	typedef lent_reference_wrapper<msg_exchange_room> msg_exchange_room_lent_ref;

public:
	async_message_loop(sender_id i_id);
	template<typename Callable, typename MessageType>
	detail::connection_base& connect(intrusive_ptr<signal_functor<Callable>> i_caller, lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_messageQueue);
	void disconnect(const detail::connection_base& i_caller);
	void clear(receiver_id i_id);
	void clear();
	void push_message(const BuiltInMessageType& i_msg);

private:
	const sender_id m_senderId;
	linked_list<msg_exchange_room_unique_ref> m_receivers;
	mutable mutex m_receiverMutex;
};

}

#include "ddk_async_message_loop.inl"
