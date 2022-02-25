#pragma once

#include "ddk_intrusive_list.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_signal_functor.h"
#include "ddk_async_message_queue.h"

namespace ddk
{
namespace detail
{

template<typename MessageType>
struct async_message_exchange_room
{
public:
	async_message_exchange_room(sender_id i_id);
	virtual ~async_message_exchange_room() = default;
	sender_id get_id() const;

	virtual void push_message(const MessageType& i_msg) = 0;
	virtual bool tryRemoveCaller(const detail::connection_base&) = 0;
	virtual bool operator==(receiver_id i_id) const = 0;

private:
	const sender_id m_id;
};

template<typename Callable, typename MessageType, typename BuiltInMessageType>
struct async_message_exchange_typed_room : async_message_exchange_room<BuiltInMessageType>
{
	typedef signal_functor<Callable> signal_functor_t;
    using async_message_exchange_room<BuiltInMessageType>::get_id;

public:
	async_message_exchange_typed_room(sender_id i_id, const intrusive_ptr<signal_functor_t>& i_caller, lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_msgQueue);
	~async_message_exchange_typed_room();
	void broadcast_msg(const MessageType& i_msg);
	void addCaller(const intrusive_ptr<signal_functor_t>& i_caller);
	bool tryRemoveCaller(const detail::connection_base& i_caller);
	void push_message(const BuiltInMessageType& i_msg) override;
	bool operator==(receiver_id i_id) const override;

private:
	intrusive_list<signal_functor_t> m_callers;
	lent_reference_wrapper<async_attachable_message_queue<MessageType>> m_msgQueue;
};

}
}

#include "ddk_async_message_exchange_room.inl"