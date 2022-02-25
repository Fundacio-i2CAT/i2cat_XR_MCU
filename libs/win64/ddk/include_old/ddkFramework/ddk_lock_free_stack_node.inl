
namespace ddk
{

template<typename T>
lock_free_stack_node<T>::lock_free_stack_node(const leaf_node_t&)
: m_nextNode(nullptr)
{
}
template<typename T>
lock_free_stack_node<T>::lock_free_stack_node(const divider_node_t&)
: m_nextNode(nullptr)
{
	m_nextNode.set(tagged_pointer<lock_free_stack_node<T>>::tag_pointer(nullptr,0x01));
}
template<typename T>
template<typename ... Args>
lock_free_stack_node<T>::lock_free_stack_node(Args&& ... i_args)
: m_nextNode(nullptr)
{
	m_arena.template construct<T>(std::forward<Args>(i_args) ...);
}
template<typename T>
lock_free_stack_node<T>::~lock_free_stack_node()
{
	m_arena.template destroy<T>();
}
template<typename T>
void lock_free_stack_node<T>::set_next(lock_free_stack_node<T>* i_nextNode)
{
	const short k_tag = tagged_pointer<lock_free_stack_node<T>>::get_tag(m_nextNode.get());

	m_nextNode.set(tagged_pointer<lock_free_stack_node<T>>::tag_pointer(i_nextNode,k_tag));
}
template<typename T>
bool lock_free_stack_node<T>::compareAndExchangeNextNode(lock_free_stack_node<T>* i_oldNode,lock_free_stack_node<T>* i_newNode)
{
	const short k_tag = tagged_pointer<lock_free_stack_node<T>>::get_tag(m_nextNode.get());

	//JAUME: we use size_t atomic instead of pointer atomics since we are dealing with tagged pointers (not aligned) and documentation explicitly states that atomic operations over not aligned pointers is UB
	return ddk::atomic_compare_exchange(m_nextNode.as_number(),tagged_pointer<lock_free_stack_node<T>>::tag_pointer_as_value(i_oldNode,k_tag),tagged_pointer<lock_free_stack_node<T>>::tag_pointer_as_value(i_newNode,k_tag));
}
template<typename T>
const lock_free_stack_node<T>* lock_free_stack_node<T>::get_next() const
{
	return tagged_pointer<const lock_free_stack_node<T>>::untag_pointer(m_nextNode.get());
}
template<typename T>
lock_free_stack_node<T>* lock_free_stack_node<T>::get_next()
{
	return tagged_pointer<lock_free_stack_node<T>>::untag_pointer(m_nextNode.get());
}
template<typename T>
template<typename TT>
void lock_free_stack_node<T>::set_value(TT&& i_value)
{
	m_arena.template set_value<T>(std::forward<TT>(i_value));
}
template<typename T>
const T& lock_free_stack_node<T>::get_value() const
{
	return m_arena.template get<T>();
}
template<typename T>
T lock_free_stack_node<T>::extract_value()
{
	return m_arena.template extract<T>();
}
template<typename T>
void lock_free_stack_node<T>::set_divider(bool i_divider)
{
	m_nextNode.set(tagged_pointer<lock_free_stack_node<T>>::tag_pointer(tagged_pointer<lock_free_stack_node<T>>::untag_pointer(m_nextNode.get()),(i_divider) ? 0x01 : 0x00));
}
template<typename T>
bool lock_free_stack_node<T>::is_divider() const
{
	return tagged_pointer<const lock_free_stack_node<T>>::is_tagged(m_nextNode.get());
}
template<typename T>
bool lock_free_stack_node<T>::is_void() const
{
	return m_arena.empty();
}

}