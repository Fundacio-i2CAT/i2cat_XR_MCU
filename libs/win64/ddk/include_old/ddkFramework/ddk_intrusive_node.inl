
#include "ddk_container_exceptions.h"
#include "ddk_allocator_exceptions.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename T>
template<typename ... Args>
intrusive_node_impl<T>::intrusive_node_impl(Args&& ... i_args)
: m_value(std::forward<Args>(i_args) ...)
{
}
template<typename T>
intrusive_node_impl<T>::~intrusive_node_impl()
{
}
template<typename T>
typename intrusive_node_impl<T>::intrusive_node_ptr intrusive_node_impl<T>::get_prev_node()
{
	return m_prevNode;
}
template<typename T>
typename intrusive_node_impl<T>::intrusive_node_const_ptr intrusive_node_impl<T>::get_prev_node() const
{
	return m_prevNode;
}
template<typename T>
typename intrusive_node_impl<T>::intrusive_node_ptr intrusive_node_impl<T>::get_next_node()
{
	return m_nextNode;
}
template<typename T>
typename intrusive_node_impl<T>::intrusive_node_const_ptr intrusive_node_impl<T>::get_next_node() const
{
	return m_nextNode;
}
template<typename T>
typename intrusive_node_impl<T>::intrusive_node_ptr intrusive_node_impl<T>::extract_prev_node()
{
	intrusive_node_ptr res = m_prevNode;

	if(m_prevNode)
	{
		m_prevNode->m_nextNode = nullptr;
		m_prevNode = nullptr;
	}

	return res;
}
template<typename T>
typename intrusive_node_impl<T>::intrusive_node_ptr intrusive_node_impl<T>::extract_next_node()
{
	intrusive_node_ptr res = m_nextNode;

	if(m_nextNode)
	{
		m_nextNode->m_prevNode = nullptr;
		m_nextNode = nullptr;
	}

	return res;
}
template<typename T>
void intrusive_node_impl<T>::clear()
{
	if(m_nextNode)
	{
		m_nextNode->clear();
		m_nextNode = nullptr;
	}
	m_prevNode = nullptr;
}
template<typename T>
void intrusive_node_impl<T>::clear_prev_node()
{
	m_prevNode = nullptr;
}
template<typename T>
void intrusive_node_impl<T>::clear_next_node()
{
	m_nextNode = nullptr;
}
template<typename T>
void intrusive_node_impl<T>::set_prev_node(const intrusive_node_ptr& i_node)
{
	if(get_raw_ptr(m_prevNode) != this)
	{
		m_prevNode = i_node;
		m_prevNode->m_nextNode = this->ref_from_this();
	}
	else
	{
		DDK_FAIL("Auto pointing!");
	}
}
template<typename T>
void intrusive_node_impl<T>::set_next_node(const intrusive_node_ptr& i_node)
{
	if(get_raw_ptr(m_nextNode) != this)
	{
		m_nextNode = i_node;
		m_nextNode->m_prevNode = this->ref_from_this();
	}
	else
	{
		DDK_FAIL("Auto pointing!");
	}
}
template<typename T>
typename intrusive_node_impl<T>::intrusive_node_ptr intrusive_node_impl<T>::collapse(intrusive_node_ptr i_node)
{
	//DDK_ASSERT(get_raw_ptr(i_prevNode->m_nextNode) == this, "Breaking linked list");
	intrusive_node_ptr res = i_node->m_nextNode;
	intrusive_node_ptr prevNode = i_node->m_prevNode;
	intrusive_node_ptr nextNode = i_node->m_nextNode;

	i_node->m_prevNode = nullptr;
	i_node->m_nextNode = nullptr;
	i_node = nullptr;

	if(prevNode)
	{
		prevNode->m_nextNode = nextNode;
	}

	if(nextNode)
	{
		nextNode->m_prevNode = prevNode;
	}

	return res;
}
template<typename T>
typename intrusive_node_impl<T>::reference intrusive_node_impl<T>::get()
{
	return m_value;
}
template<typename T>
typename intrusive_node_impl<T>::const_reference intrusive_node_impl<T>::get() const
{
	return m_value;
}
template<typename T>
typename intrusive_node_impl<T>::rreference intrusive_node_impl<T>::extract() &&
{
	return std::move(m_value);
}
template<typename T>
typename intrusive_node_impl<T>::pointer intrusive_node_impl<T>::get_ptr()
{
	return &m_value;
}
template<typename T>
typename intrusive_node_impl<T>::const_pointer intrusive_node_impl<T>::get_ptr() const
{
	return &m_value;
}
template<typename T>
unique_reference_counter* intrusive_node_impl<T>::get_reference_counter()
{
	return &m_refCounter;
}
template<typename T>
intrusive_node_impl<T>::operator T&()
{
	return m_value;
}
template<typename T>
intrusive_node_impl<T>::operator const T&() const
{
	return m_value;
}

}

template<typename T,typename Allocator>
intrusive_node<T,Allocator>::intrusive_node(intrusive_node&& other)
: m_impl(std::move(other.m_impl))
{
}
template<typename T,typename Allocator>
template<typename ... Args>
intrusive_node<T,Allocator>::intrusive_node(Args&& ... i_args)
{
	if(void* mem = m_allocator.allocate(1,sizeof(detail::intrusive_node_impl<T>)))
	{
		typedef tagged_pointer<unique_reference_counter> tagged_reference_counter;

		detail::intrusive_node_impl<T>* impl = new(mem) detail::intrusive_node_impl<T>(std::forward<Args>(i_args) ...);

		m_impl = as_unique_reference(impl,tagged_reference_counter(impl->get_reference_counter(),ReferenceAllocationType::Embedded),get_reference_wrapper_deleter(m_allocator));
	}
	else
	{
		throw bad_allocation_exception{"Could not allocate for intrusive node"};
	}
}
template<typename T,typename Allocator>
intrusive_node<T,Allocator>::~intrusive_node()
{
}
template<typename T,typename Allocator>
intrusive_node<T,Allocator>& intrusive_node<T,Allocator>::operator=(intrusive_node&& other)
{
	m_impl = std::move(m_impl);

	return *this;
}
template<typename T,typename Allocator>
void intrusive_node<T,Allocator>::clear()
{
	m_impl = nullptr;
}
template<typename T,typename Allocator>
typename intrusive_node<T,Allocator>::reference intrusive_node<T,Allocator>::get()
{
	if(m_impl)
	{
		return m_impl->get();
	}
	else
	{
		throw bad_access_exception{"Trying to access to empty intrusive node"};
	}
}
template<typename T,typename Allocator>
typename intrusive_node<T,Allocator>::const_reference intrusive_node<T,Allocator>::get() const
{
	if(m_impl)
	{
		return m_impl->get();
	}
	else
	{
		throw bad_access_exception{ "Trying to access to empty intrusive node" };
	}
}
template<typename T,typename Allocator>
typename intrusive_node<T,Allocator>::rreference intrusive_node<T,Allocator>::extract() &&
{
	if(unique_pointer_wrapper<detail::intrusive_node_impl<T>> impl = std::move(m_impl))
	{
		return impl->extract();
	}
	else
	{
		throw bad_access_exception{ "Trying to access to empty intrusive node" };
	}
}
template<typename T,typename Allocator>
typename intrusive_node<T,Allocator>::pointer intrusive_node<T,Allocator>::operator->()
{
	if(m_impl)
	{
		return m_impl->get_ptr();
	}
	else
	{
		throw bad_access_exception{ "Trying to access to empty intrusive node" };
	}
}
template<typename T,typename Allocator>
typename intrusive_node<T,Allocator>::const_pointer intrusive_node<T,Allocator>::operator->() const
{
	if(m_impl)
	{
		return m_impl->get_ptr();
	}
	else
	{
		throw bad_access_exception{ "Trying to access to empty intrusive node" };
	}
}
template<typename T,typename Allocator>
bool intrusive_node<T,Allocator>::operator==(const T& other) const
{
	return (m_impl) ? *m_impl == other : false;
}
template<typename T,typename Allocator>
bool intrusive_node<T,Allocator>::empty() const
{
	return m_impl == nullptr;
}
template<typename T,typename Allocator>
intrusive_node<T,Allocator>::operator bool() const
{
	return m_impl != nullptr;
}

}
