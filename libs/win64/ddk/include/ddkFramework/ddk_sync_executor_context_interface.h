#pragma once

#include "ddk_function.h"
#include "ddk_weak_pointer_wrapper.h"
#include "ddk_shared_pointer_wrapper.h"

namespace ddk
{

struct continuation_token
{
public:
	static const size_t ntoken = -1;

	continuation_token(size_t i_token = ntoken);
	continuation_token(const continuation_token&) = delete;
	continuation_token(continuation_token&& other);

	continuation_token& operator=(const continuation_token&) = delete;
	continuation_token& operator=(continuation_token&& other);
	bool operator==(const continuation_token& other) const;
	bool operator==(const size_t& other) const;
	operator bool() const;

private:
	size_t m_id;
};

class executor_context_interface
{
public:
	virtual ~executor_context_interface() = default;

	virtual void start(const function<void()>&) = 0;
	virtual continuation_token enqueue(const function<void()>&, unsigned char) = 0;
	virtual bool dismiss(unsigned char, continuation_token) = 0;
	virtual void clear() = 0;
};

typedef weak_pointer_wrapper<executor_context_interface> executor_context_weak_ptr;
typedef weak_pointer_wrapper<const executor_context_interface> executor_context_const_weak_ptr;

typedef shared_pointer_wrapper<executor_context_interface> executor_context_shared_ptr;
typedef shared_pointer_wrapper<const executor_context_interface> executor_context_const_shared_ptr;

typedef lent_reference_wrapper<executor_context_interface> executor_context_lent_ref;
typedef lent_reference_wrapper<const executor_context_interface> executor_context_const_lent_ref;
typedef lent_pointer_wrapper<executor_context_interface> executor_context_lent_ptr;
typedef lent_pointer_wrapper<const executor_context_interface> executor_context_const_lent_ptr;

}