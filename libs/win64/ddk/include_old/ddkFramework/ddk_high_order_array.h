#pragma once

#include "ddk_iterable_defs.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename,size_t ...>
class high_order_sub_array;

template<typename T, size_t rank, size_t ... ranks>
class high_order_sub_array<T,rank,ranks...>
{
public:
	high_order_sub_array(T& i_ref);
	high_order_sub_array(const high_order_sub_array& other) = delete;
	high_order_sub_array(high_order_sub_array&& other) = delete;
	high_order_sub_array<T,ranks...> operator[](size_t index);
	high_order_sub_array<const T,ranks...> operator[](size_t index) const;
    static inline constexpr size_t size();

private:
	T& m_ref;
};

template<typename T>
class high_order_sub_array<T>
{
public:
	high_order_sub_array(T& i_ref);
	high_order_sub_array(const high_order_sub_array& other) = delete;
	high_order_sub_array(high_order_sub_array&& other) = delete;

	template<typename TT>
	high_order_sub_array& operator=(TT&& i_value);
	operator T&();
	operator const T&() const;
    static inline constexpr size_t size();

private:
	T& m_ref;
};

}

template<typename T, size_t rank, size_t ... ranks>
class high_order_array
{
    template<typename TT, size_t rrank, size_t ... rranks>
    friend class high_order_array;

	struct storage
	{
		T& operator[](size_t i_index);
		const T& operator[](size_t i_index) const;

		T m_data[mpl::prod_ranks<rank,ranks...>];
	};

public:
	typedef T place_type;
	static constexpr size_t num_places = mpl::prod_ranks<rank,ranks...>;
	static const size_t s_numRanks = mpl::num_ranks<rank,ranks...>;
	static const size_t s_totalSize = mpl::prod_ranks<rank,ranks...>;

	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;

	constexpr high_order_array() = default;
	constexpr high_order_array(const high_order_array<T,rank,ranks...>& other) = default;
    TEMPLATE(typename ... Args)
    REQUIRES(IS_NUMBER_OF_ARGS(num_places,Args...),IS_CONSTRUCTIBLE(T,Args)...)
    constexpr high_order_array(Args&& ... i_args);
	TEMPLATE(typename TT)
	REQUIRES(IS_CONSTRUCTIBLE(T,TT))
	constexpr high_order_array(const high_order_array<TT,rank,ranks...>& other);
	~high_order_array() = default;
	detail::high_order_sub_array<T,ranks...> operator[](size_t index);
	constexpr detail::high_order_sub_array<const T,ranks...> operator[](size_t index) const;
	reference at(const high_order_array<size_t,s_numRanks>& i_indexs);
	constexpr const_reference at(const high_order_array<size_t,s_numRanks>& i_indexs) const;
	constexpr reference at(size_t i_index);
	constexpr const_reference at(size_t i_index) const;
	high_order_array& operator=(const high_order_array<T,rank,ranks...>& other);
	template<typename TT>
	high_order_array& operator=(const high_order_array<TT,rank,ranks...>& other);
	constexpr size_t size() const;
	constexpr bool empty() const;

private:
	storage m_data;
};

}

#include "ddk_high_order_array.inl"
#include "ddk_high_order_array_adaptor.h"
