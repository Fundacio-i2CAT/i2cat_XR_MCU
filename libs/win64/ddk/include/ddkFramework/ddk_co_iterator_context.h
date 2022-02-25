#pragma once

#include <cstddef>

namespace ddk
{
namespace detail
{

struct co_forward_iterator_context_impl
{
public:
	static const size_t npos = -1;

	co_forward_iterator_context_impl(size_t i_currIndex = npos);

	void incr();
	void accept();
	void reject();
	void close();

	size_t get_current() const;
	size_t get_target() const;

protected:
	size_t m_currIndex = npos;
	size_t m_targetIndex = npos;
};

struct co_bidirectional_iterator_context_impl : co_forward_iterator_context_impl
{
public:
	using co_forward_iterator_context_impl::co_forward_iterator_context_impl;

	void decr();
};

struct co_random_access_iterator_context_impl : co_bidirectional_iterator_context_impl
{
public:
	using co_bidirectional_iterator_context_impl::co_bidirectional_iterator_context_impl;

	void shift(int i_shift);
	void set_abs(size_t i_index);
};

}

struct co_forward_iterator_context
{
public:
	co_forward_iterator_context(const detail::co_forward_iterator_context_impl* i_impl);

	size_t get_curr_index() const;
	bool go_forward() const;

private:
	const detail::co_forward_iterator_context_impl& m_impl;
};

struct co_bidirectional_iterator_context
{
public:
	co_bidirectional_iterator_context(const detail::co_bidirectional_iterator_context_impl* i_impl);

	size_t get_curr_index() const;
	bool go_forward() const;
	bool go_backward() const;

private:
	const detail::co_bidirectional_iterator_context_impl& m_impl;
};

struct co_random_access_iterator_context
{
public:
	co_random_access_iterator_context(const detail::co_random_access_iterator_context_impl* i_impl);

	size_t get_curr_index() const;
	int shift() const;

private:
	const detail::co_random_access_iterator_context_impl& m_impl;
};

}
