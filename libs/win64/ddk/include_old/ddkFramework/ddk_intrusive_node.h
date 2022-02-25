#pragma once

#include "ddk_unique_pointer_wrapper.h"
#include "ddk_lend_from_this.h"
#include "ddk_system_allocator.h"

namespace ddk
{
namespace detail
{

template<typename T>
class intrusive_node_impl : public lend_from_this<intrusive_node_impl<T>>
{
public:
	typedef lent_pointer_wrapper<intrusive_node_impl> intrusive_node_ptr;
	typedef lent_pointer_wrapper<const intrusive_node_impl> intrusive_node_const_ptr;
	typedef T value_type;
	typedef typename std::add_lvalue_reference<value_type>::type reference;
	typedef typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type const_reference;
	typedef typename std::add_rvalue_reference<value_type>::type rreference;
	typedef typename std::add_pointer<value_type>::type pointer;
	typedef typename std::add_pointer<typename std::add_const<value_type>::type>::type const_pointer;

	template<typename ... Args>
	intrusive_node_impl(Args&& ... i_args);
	intrusive_node_impl(const intrusive_node_impl&) = delete;
	intrusive_node_impl(intrusive_node_impl&&) = delete;
	~intrusive_node_impl();
	intrusive_node_impl& operator=(const intrusive_node_impl&) = delete;
	intrusive_node_impl& operator=(intrusive_node_impl&&) = delete;
	inline intrusive_node_ptr get_prev_node();
	inline intrusive_node_const_ptr get_prev_node() const;
	inline intrusive_node_ptr get_next_node();
	inline intrusive_node_const_ptr get_next_node() const;
	inline intrusive_node_ptr extract_prev_node();
	inline intrusive_node_ptr extract_next_node();
	inline void clear();
	inline void clear_prev_node();
	inline void clear_next_node();
	inline void set_prev_node(const intrusive_node_ptr& i_node);
	inline void set_next_node(const intrusive_node_ptr& i_node);
	static intrusive_node_ptr collapse(intrusive_node_ptr i_node);
	inline reference get();
	inline const_reference get() const;
	inline rreference extract() &&;
	inline pointer get_ptr();
	inline const_pointer get_ptr() const;
	unique_reference_counter* get_reference_counter();
	operator T&();
	operator const T&() const;

private:
	intrusive_node_ptr m_prevNode = nullptr;
	intrusive_node_ptr m_nextNode = nullptr;
	T m_value;
	unique_reference_counter m_refCounter;
};

}

template<typename T, typename Allocator = system_allocator>
struct intrusive_node
{
	friend inline lent_pointer_wrapper<detail::intrusive_node_impl<T>> make_intrusive(intrusive_node& i_node)
	{
		return lend(i_node.m_impl);
	}

public:
	typedef T value_type;
	typedef typename std::add_lvalue_reference<value_type>::type reference;
	typedef typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type const_reference;
	typedef typename std::add_rvalue_reference<value_type>::type rreference;
	typedef typename std::add_const<rreference>::type const_rreference;
	typedef typename std::add_pointer<value_type>::type pointer;
	typedef typename std::add_pointer<typename std::add_const<value_type>::type>::type const_pointer;

	intrusive_node() = default;
	intrusive_node(const intrusive_node& other) = delete;
	intrusive_node(intrusive_node&& other);
	template<typename ... Args>
	intrusive_node(Args&& ... i_args);
	~intrusive_node();
	intrusive_node& operator=(const intrusive_node& other) = delete;
	intrusive_node& operator=(intrusive_node&& other);
	inline void clear();
	inline reference get();
	inline const_reference get() const;
	inline rreference extract() &&;
	inline pointer operator->();
	inline const_pointer operator->() const;
	inline bool operator==(const T& other) const;
	inline bool empty() const;
	explicit operator bool() const;

private:
	unique_pointer_wrapper<detail::intrusive_node_impl<T>> m_impl;
	Allocator m_allocator;
};

template<typename T>
using intrusive_ref = lent_reference_wrapper<detail::intrusive_node_impl<T>>;
template<typename T>
using intrusive_const_ref = lent_reference_wrapper<const detail::intrusive_node_impl<T>>;
template<typename T>
using intrusive_ptr = lent_pointer_wrapper<detail::intrusive_node_impl<T>>;
template<typename T>
using intrusive_const_ptr = lent_pointer_wrapper<const detail::intrusive_node_impl<T>>;

}

#include "ddk_intrusive_node.inl"