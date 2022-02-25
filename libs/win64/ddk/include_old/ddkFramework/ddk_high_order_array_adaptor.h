#pragma once

#include "ddk_high_order_array.h"
#include "ddk_optional.h"
#include "ddk_iterable_action_defs.h"
#include <utility>

namespace ddk
{

template<typename T, size_t ... ranks>
class iterable_adaptor<high_order_array<T,ranks ...>>
{
public:
	typedef typename high_order_array<T,ranks...>::value_type value_type;
	typedef typename high_order_array<T,ranks...>::reference reference;
	typedef typename high_order_array<T,ranks...>::const_reference const_reference;
	typedef long long difference_type;

	iterable_adaptor(high_order_array<T,ranks...>& i_iterable,const ddk::shift_action& i_initialAction);
	template<typename Sink>
	inline difference_type forward_next_value_in(Sink&& i_sink);
	template<typename Sink>
	inline difference_type forward_next_value_in(Sink&& i_sink) const;
	template<typename Sink>
	inline difference_type forward_prev_value_in(Sink&& i_sink);
	template<typename Sink>
	inline difference_type forward_prev_value_in(Sink&& i_sink) const;
	template<typename Sink>
	inline difference_type forward_shift_value_in(difference_type i_shift,Sink&& i_sink);
	template<typename Sink>
	inline difference_type forward_shift_value_in(difference_type i_shift,Sink&& i_sink) const;
	inline bool valid() const noexcept;

private:
	static const size_t s_numRanks = mpl::get_num_ranks<ranks...>();
	static constexpr size_t s_ranks[s_numRanks] = { ranks ... };

	high_order_array<T,ranks ...>& m_iterable;
	mutable size_t m_currIndex = 0;
};

template<typename T, size_t ... ranks>
class iterable_adaptor<const high_order_array<T,ranks ...>>
{
public:
	typedef typename high_order_array<T,ranks...>::value_type value_type;
	typedef typename high_order_array<T,ranks...>::const_reference reference;
	typedef typename high_order_array<T,ranks...>::const_reference const_reference;
	typedef long long difference_type;

	iterable_adaptor(const high_order_array<T,ranks...>& i_iterable,const ddk::shift_action& i_initialAction);
	template<typename Sink>
	inline difference_type forward_next_value_in(Sink&& i_sink);
	template<typename Sink>
	inline difference_type forward_next_value_in(Sink&& i_sink) const;
	template<typename Sink>
	inline difference_type forward_prev_value_in(Sink&& i_sink);
	template<typename Sink>
	inline difference_type forward_prev_value_in(Sink&& i_sink) const;
	template<typename Sink>
	inline difference_type forward_shift_value_in(difference_type i_shift,Sink&& i_sink);
	template<typename Sink>
	inline difference_type forward_shift_value_in(difference_type i_shift,Sink&& i_sink) const;
	inline bool valid() const noexcept;

private:
	static const size_t s_numRanks = mpl::get_num_ranks<ranks...>();
	static constexpr size_t s_ranks[s_numRanks] = { ranks ... };

	const high_order_array<T,ranks ...>& m_iterable;
	mutable size_t m_currIndex = 0;
};

}

#include "ddk_high_order_array_adaptor.inl"
