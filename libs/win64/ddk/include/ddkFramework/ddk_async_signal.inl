
namespace ddk
{

template<typename ... Types>
async_signal<void(Types...)>::async_signal(const std::chrono::milliseconds& i_updateTime)
: m_id(reinterpret_cast<size_t>(this))
, m_msgLoop(m_id)
, m_msgQueue(make_executor<thread_polling_executor>(i_updateTime))
{
}
template<typename ... Types>
async_signal<void(Types...)>::async_signal(thread_executor_unique_ref i_executor)
: m_id(reinterpret_cast<size_t>(this))
, m_msgLoop(m_id)
, m_msgQueue(std::move(i_executor))
{
}
template<typename ... Types>
detail::connection_base& async_signal<void(Types...)>::connect(const ddk::function<void(Types...)>& i_function) const
{
	return m_msgLoop.connect(make_intrusive(m_callers.push(i_function,static_cast<const detail::signal_connector&>(*this))),ddk::lend(m_msgQueue));
}
template<typename ... Types>
template<typename MessageType>
detail::connection_base& async_signal<void(Types...)>::connect(const ddk::function<void(Types...)>& i_function,lent_reference_wrapper<async_attachable_message_queue<MessageType>> i_messageQueue) const
{
	return m_msgLoop.connect(make_intrusive(m_callers.push(i_function,static_cast<const detail::signal_connector&>(*this))),i_messageQueue);
}
template<typename ... Types>
void async_signal<void(Types...)>::disconnect()
{
	m_msgLoop.clear();
	m_callers.clear();
}
template<typename ... Types>
template<typename ... Args>
void async_signal<void(Types...)>::execute(Args&& ... i_args) const
{
	m_msgLoop.push_message(builtn_message_type(m_id,std::forward<Args>(i_args) ...));
}
template<typename ... Types>
void async_signal<void(Types...)>::disconnect(const detail::connection_base& i_base) const
{
	typedef typename linked_list<intrusive_node<signal_functor_t>>::iterator iterator;
	iterator itCaller = std::find_if(m_callers.begin(),m_callers.end(),[&i_base](const intrusive_node<signal_functor_t>& i_functor) { return i_functor->get_id() == i_base.get_id(); });

	if(itCaller != m_callers.end())
	{
		m_msgLoop.disconnect((*itCaller).get());

		m_callers.erase(std::move(itCaller));
	}
}

}