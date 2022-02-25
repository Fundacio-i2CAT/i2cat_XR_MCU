
namespace ddk
{

template<typename BuiltInMessageType>
async_message_loop<BuiltInMessageType>::async_message_loop(sender_id i_id)
: m_senderId(i_id)
{
}
template<typename BuiltInMessageType>
template<typename Callable,typename MessageType>
detail::connection_base& async_message_loop<BuiltInMessageType>::connect(intrusive_ptr<signal_functor<Callable>> i_caller,lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_messageQueue)
{
	detail::connection_base& res = *i_caller;

	mutex_guard lg(m_receiverMutex);

	typename linked_list<msg_exchange_room_unique_ref>::iterator itReceiver = std::find_if(m_receivers.begin(),m_receivers.end(),[&i_messageQueue](const msg_exchange_room_unique_ref& i_receiver) { return *i_receiver == i_messageQueue->get_id(); });

	if(itReceiver == m_receivers.end())
	{
		m_receivers.push(make_unique_reference<detail::async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>>(m_senderId,i_caller,i_messageQueue));
	}
	else if(lent_pointer_wrapper<detail::async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>> room = dynamic_lent_cast<detail::async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>>(lend(*itReceiver)))
	{
		room->addCaller(i_caller);
	}

	return res;
}
template<typename BuiltInMessageType>
void async_message_loop<BuiltInMessageType>::disconnect(const detail::connection_base& i_caller)
{
	typename linked_list<msg_exchange_room_unique_ref>::iterator itReceiver = m_receivers.begin();
	for(; itReceiver != m_receivers.end(); ++itReceiver)
	{
		if((*itReceiver)->tryRemoveCaller(i_caller))
		{
			return;
		}
	}

	DDK_FAIL("Trying to remove caller with no receiver associated");
}
template<typename BuiltInMessageType>
void async_message_loop<BuiltInMessageType>::clear(receiver_id i_id)
{
	mutex_guard lg(m_receiverMutex);

	typename linked_list<msg_exchange_room_unique_ref>::iterator itReceiver = std::find_if(m_receivers.begin(),m_receivers.end(),[&i_id](const msg_exchange_room_unique_ref& i_receiver) { i_receiver->get_id() == i_id; });

	if(itReceiver != m_receivers.end())
	{
		m_receivers.erase(itReceiver);
	}
}
template<typename BuiltInMessageType>
void async_message_loop<BuiltInMessageType>::clear()
{
	mutex_guard lg(m_receiverMutex);

	m_receivers.clear();
}
template<typename BuiltInMessageType>
void async_message_loop<BuiltInMessageType>::push_message(const BuiltInMessageType& i_msg)
{
	mutex_guard lg(m_receiverMutex);

	typename linked_list<msg_exchange_room_unique_ref>::iterator itReceiver = m_receivers.begin();
	for(; itReceiver != m_receivers.end(); ++itReceiver)
	{
		msg_exchange_room_lent_ref room = lend(*itReceiver);

		room->push_message(i_msg);
	}
}

}
