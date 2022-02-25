
namespace ddk
{
namespace detail
{

template<typename MessageType>
async_message_exchange_room<MessageType>::async_message_exchange_room(sender_id i_id)
: m_id(i_id)
{
}
template<typename MessageType>
sender_id async_message_exchange_room<MessageType>::get_id() const
{
	return m_id;
}

template<typename Callable,typename MessageType,typename BuiltInMessageType>
async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>::async_message_exchange_typed_room(sender_id i_id,const intrusive_ptr<signal_functor_t>& i_caller,lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_msgQueue)
: async_message_exchange_room<BuiltInMessageType>(i_id)
, m_msgQueue(i_msgQueue)
{
	m_callers.push(i_caller);
	m_msgQueue->start(i_id,make_function(this,&async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>::broadcast_msg));
}
template<typename Callable,typename MessageType,typename BuiltInMessageType>
async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>::~async_message_exchange_typed_room()
{
	m_msgQueue->stop(get_id());
}
template<typename Callable,typename MessageType,typename BuiltInMessageType>
void async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>::broadcast_msg(const MessageType& i_msg)
{
	DDK_ASSERT(get_id() == i_msg.get_id(),"Forwarding unrelated messages");

	const typename MessageType::tuple_t& forwarded_args = i_msg.forward_message();

	typename intrusive_list<signal_functor_t>::const_iterator itCaller = m_callers.begin();
	for(; itCaller != m_callers.end(); ++itCaller)
	{
		if(itCaller->is_enabled())
		{
			itCaller->execute_tuple(forwarded_args);
		}
	}
}
template<typename Callable,typename MessageType,typename BuiltInMessageType>
void async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>::addCaller(const intrusive_ptr<signal_functor_t>& i_caller)
{
	m_callers.push(i_caller);
}
template<typename Callable,typename MessageType,typename BuiltInMessageType>
bool async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>::tryRemoveCaller(const detail::connection_base& i_caller)
{
	typename intrusive_list<signal_functor_t>::iterator itCaller = std::find_if(m_callers.begin(),m_callers.end(),[&i_caller](const signal_functor_t& i_functor) { return i_functor.get_id() == i_caller.get_id(); });

	if(itCaller != m_callers.end())
	{
		m_callers.erase(itCaller);

		return true;
	}
	else
	{
		return false;
	}
}
template<typename Callable,typename MessageType,typename BuiltInMessageType>
void async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>::push_message(const BuiltInMessageType& i_msg)
{
	m_msgQueue->push_message(MessageType(i_msg));
}
template<typename Callable,typename MessageType,typename BuiltInMessageType>
bool async_message_exchange_typed_room<Callable,MessageType,BuiltInMessageType>::operator==(receiver_id i_id) const
{
	return m_msgQueue->get_id() == i_id;
}

}
}