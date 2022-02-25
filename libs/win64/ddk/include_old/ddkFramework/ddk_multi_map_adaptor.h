#pragma once

#include "ddk_multi_map.h"
#include "ddk_iterable_action_defs.h"
#include "ddk_awaitable.h"

namespace ddk
{

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
class iterable_adaptor<multi_map<Key,Value,Map,Allocator>>
{
	typedef typename multi_map<Key,Value,Map,Allocator>::value_t value_t;
	typedef typename value_t::iterator iterator;

public:
	typedef std::pair<const std::vector<Key>,Value&> value_type;
	typedef std::pair<const std::vector<Key>,Value&>& reference;
	typedef std::pair<const std::vector<Key>,const Value&>& const_reference;
	typedef long long difference_type;

	iterable_adaptor(multi_map<Key,Value,Map,Allocator>& i_iterable,const ddk::shift_action& i_initialAction);
	template<typename Sink>
	inline difference_type forward_next_value_in(Sink&& i_sink);
	template<typename Sink>
	inline difference_type forward_next_value_in(Sink&& i_sink) const;
	template<typename Sink>
	inline difference_type forward_prev_value_in(Sink&& i_sink);
	template<typename Sink>
	inline difference_type forward_prev_value_in(Sink&& i_sink) const;
	inline bool valid() const noexcept;

private:
	reference navigate(const ddk::shift_action& i_initailAction);
	void _navigate(value_t& i_map, std::vector<Key>& i_preffix);
	
	multi_map<Key,Value,Map,Allocator>& m_iterable;
	awaitable<reference> m_awaitable;
	difference_type m_nextMov;
	bool m_valid = true;
};

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
class iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>
{
	typedef typename multi_map<Key,Value,Map,Allocator>::value_t value_t;
	typedef typename value_t::iterator iterator;

public:
	typedef std::pair<const std::vector<Key>,Value&> value_type;
	typedef std::pair<const std::vector<Key>,Value&>& reference;
	typedef std::pair<const std::vector<Key>,const Value&>& const_reference;
	typedef long long difference_type;

	iterable_adaptor(const multi_map<Key,Value,Map,Allocator>& i_iterable,const ddk::shift_action& i_initialAction);
	template<typename Sink>
	inline difference_type forward_next_value_in(Sink&& i_sink);
	template<typename Sink>
	inline difference_type forward_next_value_in(Sink&& i_sink) const;
	template<typename Sink>
	inline difference_type forward_prev_value_in(Sink&& i_sink);
	template<typename Sink>
	inline difference_type forward_prev_value_in(Sink&& i_sink) const;
	inline bool valid() const noexcept;

private:
	const_reference navigate(const ddk::shift_action& i_initailAction);
	void _navigate(const value_t& i_map, std::vector<Key>& i_preffix);

	const multi_map<Key,Value,Map,Allocator>& m_iterable;
	awaitable<const_reference> m_awaitable;
	difference_type m_nextMov;
	bool m_valid = true;
};

}

#include "ddk_multi_map_adaptor.inl"