#pragma once

#include "ddk_future.h"
#include "ddk_iterable.h"
#include "ddk_container_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace trans
{
namespace detail
{

template<typename Sink>
struct linearized_index_access_dumping
{
public:
	linearized_index_access_dumping(Sink& i_sink);
	template<typename Value>
	inline void apply(Value&& i_value) const;

private:
	Sink& m_sink;
	mutable size_t m_currIndex = 0;
};
TEMPLATE(typename Sink,typename ValueType)
REQUIRES(ASSIGNABLE_FIXED_SIZE_CONTAINER_BY_INDEX_ACCESS(Sink,ValueType))
linearized_index_access_dumping<Sink> iterable_dumper_resolver(Sink&,const ValueType&);

template<typename Sink>
struct dynamic_access_dumping
{
public:
	dynamic_access_dumping(Sink& i_sink);
	template<typename Value>
	inline void apply(Value&& i_value) const;

private:
	Sink& m_sink;
};
TEMPLATE(typename Sink,typename ValueType)
REQUIRES(ASSIGNABLE_DYNAMIC_CONTAINER_BY_INDEX_ACCESS(Sink,ValueType))
dynamic_access_dumping<Sink> iterable_dumper_resolver(Sink&,const ValueType&);

template<typename Sink>
struct type_access_dumping
{
public:
	type_access_dumping(Sink& i_sink);
	template<typename ... T>
	inline void apply(const variant<T...>& i_value) const;

private:
	template<size_t ... Indexs,typename ... T>
	void set(const mpl::sequence<Indexs...>&, const variant<T...>& i_value);
	template<size_t Index, typename ... T>
	static void _set(Sink& i_sink, const variant<T...>& i_value);

	Sink& m_sink;
	mutable size_t m_currIndex = 0;
};
TEMPLATE(typename Sink,typename ValueType)
REQUIRES(ASSIGNABLE_FIXED_SIZE_CONTAINER_BY_TYPE_ACCESS(Sink,ValueType))
type_access_dumping<Sink> iterable_dumper_resolver(Sink&,const ValueType&);


template<typename Sink,typename ValueType>
using iterable_dumper_type = decltype(iterable_dumper_resolver(std::declval<Sink&>(),std::declval<ValueType>()));

template<typename Sink,typename Traits>
inline future<ddk::action_result> iterable_transformation_dump(Sink& i_sink, const ddk::detail::iterable<Traits>& i_transformedIterable);

}
}
}

#include "ddk_iterable_algorithm_impl.inl"
