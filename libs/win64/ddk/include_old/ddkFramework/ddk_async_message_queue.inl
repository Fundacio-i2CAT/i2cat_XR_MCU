
#include "ddk_function_utils.h"

namespace ddk
{

template<typename MessageType>
void async_message_queue<MessageType>::push_message(const MessageType& i_msg)
{
	m_list.push(i_msg);
}
template<typename MessageType>
optional<MessageType> async_message_queue<MessageType>::pop_message()
{
	return m_list.pop();
}
template<typename MessageType>
bool async_message_queue<MessageType>::empty() const
{
	return m_list.empty();
}
template<typename MessageType>
receiver_id async_message_queue<MessageType>::get_id() const
{
	return receiver_id(reinterpret_cast<size_t>(this));
}

template<typename MessageType>
async_attachable_message_queue<MessageType>::async_attachable_message_queue(thread_executor_unique_ref i_executor)
: m_executor(std::move(i_executor))
{
}
template<typename MessageType>
void async_attachable_message_queue<MessageType>::start(sender_id i_id,const ddk::function<void(const MessageType&)>& i_processor)
{
	mutex_guard lg(m_mutex);

	typename linked_list<std::pair<sender_id,ddk::function<void(const message_type&)>>>::const_iterator itReceiver = std::find_if(m_receivers.begin(),m_receivers.end(),[&i_id](const std::pair<sender_id,ddk::function<void(const message_type&)>>& i_pair) { return i_pair.first == i_id; });

	DDK_ASSERT(itReceiver == m_receivers.end(),"Attempting to connect more than once to the same queue");

	if(itReceiver == m_receivers.end())
	{
		if(m_receivers.empty())
		{
			thread_executor_interface::start_result startRes = m_executor->execute(nullptr,ddk::make_function(this,&async_attachable_message_queue<MessageType>::dispatch_messages));

			DDK_ASSERT(startRes == success,"Error while starting thread executor : " + ddk::formatter<std::string>::format(startRes.error()));
		}

		m_receivers.push(std::make_pair(i_id,i_processor));
	}
}
template<typename MessageType>
void async_attachable_message_queue<MessageType>::stop(sender_id i_id)
{
	bool toBeStopped = false;

	m_mutex.lock();

	typename linked_list<std::pair<sender_id,function<void(const message_type&)>>>::iterator itReceiver = std::find_if(m_receivers.begin(),m_receivers.end(),[&i_id](const std::pair<sender_id,function<void(const message_type&)>>& i_pair) { return i_pair.first == i_id; });

	DDK_ASSERT(itReceiver != m_receivers.end(),"Attempting to unconnect from not connected queue");

	if(itReceiver != m_receivers.end())
	{
		m_receivers.erase(std::move(itReceiver));

		toBeStopped = m_receivers.empty();
	}

	m_mutex.unlock();

	if(toBeStopped)
	{
		thread_executor_interface::resume_result stopRes = m_executor->resume();

		DDK_ASSERT(stopRes == success,"Error while starting thread executor : " + ddk::formatter<std::string>::format(stopRes.error()));
	}
}
template<typename MessageType>
void async_attachable_message_queue<MessageType>::push_message(const MessageType& i_msg)
{
	async_message_queue<MessageType>::push_message(i_msg);

	m_executor->signal();
}
template<typename MessageType>
void async_attachable_message_queue<MessageType>::dispatch_messages()
{
	mutex_guard lg(m_mutex);

	while(const optional<message_type> currMsgOpt = pop_message())
	{
		const message_type& currMsg = *currMsgOpt;

		typename linked_list<std::pair<sender_id,ddk::function<void(const message_type&)>>>::const_iterator itReceiver = m_receivers.begin();
		for(; itReceiver != m_receivers.end(); ++itReceiver)
		{
			const std::pair<const sender_id,ddk::function<void(const message_type&)>>& currReceiver = *itReceiver;

			if(currReceiver.first == currMsg.get_id())
			{
				eval(currReceiver.second,currMsg);
			}
		}
	}
}

}
