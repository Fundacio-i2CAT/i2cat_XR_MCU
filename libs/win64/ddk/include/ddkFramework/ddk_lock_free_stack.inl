
namespace ddk
{

template<typename T,typename Allocator>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Allocator,Args...))
single_consumer_lock_free_stack<T,Allocator>::single_consumer_lock_free_stack(Args&& ... i_args)
: m_head(leaf_node)
, m_tail(leaf_node)
, m_allocator(Allocator(std::forward<Args>(i_args)...))
{
	if(void* mem = m_allocator.allocate(1))
	{
		lock_free_stack_node<T>* divider = new (mem) lock_free_stack_node<T>(divider_node);

		m_head.set_next(divider);
		m_tail.set_next(divider);
	}

}
template<typename T,typename Allocator>
single_consumer_lock_free_stack<T,Allocator>::~single_consumer_lock_free_stack()
{
	clear();
}
template<typename T,typename Allocator>
void single_consumer_lock_free_stack<T,Allocator>::clear()
{
	while(empty() == false)
	{
		pop();
	}

	if(lock_free_stack_node<T>* divisorNode = m_head.get_next())
	{
		deallocate_node(divisorNode);
	}
}
template<typename T,typename Allocator>
template<typename ... Args>
void single_consumer_lock_free_stack<T,Allocator>::push(Args&& ... i_args)
{
	if(lock_free_stack_node<T>* newNode = allocate_node(std::forward<Args>(i_args) ...))
	{
		_push(newNode);
	}
}
template<typename T,typename Allocator>
optional<T> single_consumer_lock_free_stack<T,Allocator>::pop()
{
	if(empty() == false)
	{
		lock_free_stack_node<T>* firstNode = nullptr;
		lock_free_stack_node<T>* nextNode = nullptr;

		do
		{
			firstNode = m_head.get_next();

			if(firstNode->is_divider())
			{
				return none;
			}

			nextNode = firstNode->get_next();

			if(m_head.compareAndExchangeNextNode(firstNode,nextNode))
			{
				break;
			}
			else
			{
				std::this_thread::yield();
			}

		} while(true);

		T res = firstNode->extract_value();

		deallocate_node(firstNode);

		return std::move(res);
	}

	return none;
}
template<typename T,typename Allocator>
bool single_consumer_lock_free_stack<T,Allocator>::empty() const
{
	return m_head.get_next()->is_divider();
}
template<typename T,typename Allocator>
void single_consumer_lock_free_stack<T,Allocator>::_push(lock_free_stack_node<T>* i_newNode)
{
	static const int s_sleepTime = 5;
	lock_free_stack_node<T>* lastNode = nullptr;
	size_t numOfRetries = 0;

	do
	{
		lastNode = m_tail.get_next();

		if(m_tail.compareAndExchangeNextNode(lastNode,i_newNode))
		{
			break;
		}
		else
		{
			std::this_thread::yield();
		}
	} while(true);

	while(lastNode->is_divider() == false)
	{
		std::this_thread::yield();
	}

	lastNode->set_value(i_newNode->extract_value());

	lastNode->set_next(i_newNode);

	i_newNode->set_divider(true);

	lastNode->set_divider(false);
}
template<typename T,typename Allocator>
template<typename ... Args>
lock_free_stack_node<T>* single_consumer_lock_free_stack<T,Allocator>::allocate_node(Args&& ... i_args)
{
	if(void* mem = m_allocator.allocate(1))
	{
		return new (mem) lock_free_stack_node<T>(std::forward<Args>(i_args) ...);
	}
	else
	{
		return nullptr;
	}
}
template<typename T,typename Allocator>
void single_consumer_lock_free_stack<T,Allocator>::deallocate_node(lock_free_stack_node<T>* i_ptr)
{
	i_ptr->~lock_free_stack_node<T>();

	m_allocator.deallocate(i_ptr);
}

template<typename T,typename Allocator>
optional<T> multiple_consumer_lock_free_stack<T,Allocator>::pop()
{
	if(this->empty() == false)
	{
		lock_free_stack_node<T>* firstNode = nullptr;
		lock_free_stack_node<T>* nextNode = nullptr;

		do
		{
			m_barrier.lock_exclusive();

			firstNode = this->m_head.get_next();

			if(firstNode->is_divider())
			{
				this->m_barrier.unlock_exclusive();

				return none;
			}

			nextNode = firstNode->get_next();

			m_barrier.unlock_exclusive();

			if(this->m_head.compareAndExchangeNextNode(firstNode,nextNode))
			{
				break;
			}
			else
			{
				std::this_thread::yield();
			}

		} while(true);


		T res = firstNode->extract_value();

		m_barrier.lock();

		this->deallocate_node(firstNode);

		m_barrier.unlock();

		return std::move(res);
	}

	return none;
}


}
