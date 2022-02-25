#pragma once

#include "ddk_intrusive_node.h"
#include "ddk_list_iterator.h"

#define DETECT_CYCLES

namespace ddk
{

template<typename T>
struct intrusive_list
{
public:
	typedef list_iterator<intrusive_ptr<T>> iterator;
	typedef list_iterator<intrusive_const_ptr<T>> const_iterator;

	intrusive_list();
	intrusive_list(const intrusive_list<T>& other);
	intrusive_list(intrusive_list<T>&& other);
	~intrusive_list();
	void push(const intrusive_ptr<T>& i_node);
	void pop();
	void clear();
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	iterator erase(iterator i_it);
	size_t size() const;
	bool empty() const;

private:
	bool already_contains_node(intrusive_ptr<T> i_node) const;

	intrusive_ptr<T> m_firstNode;
	intrusive_ptr<T> m_lastNode;
};

}

#include "ddk_intrusive_list.inl"
