
#include "ddk_container_exceptions.h"

namespace ddk
{
namespace detail
{

template<typename T,size_t rank,size_t ... ranks>
high_order_sub_array<T,rank,ranks...>::high_order_sub_array(T& i_ref)
: m_ref(i_ref)
{
}
template<typename T,size_t rank,size_t ... ranks>
high_order_sub_array<T,ranks...> high_order_sub_array<T,rank,ranks...>::operator[](size_t i_index)
{
	if(i_index < rank)
	{
		static const size_t s_partialSize = high_order_sub_array<T,ranks...>::size();

		return *(&m_ref + i_index * s_partialSize);
	}
	else
	{
		throw bad_access_exception{ "Index out of bounds" };
	}
}
template<typename T,size_t rank,size_t ... ranks>
high_order_sub_array<const T,ranks...> high_order_sub_array<T,rank,ranks...>::operator[](size_t i_index) const
{
	if(i_index < rank)
	{
		static const size_t s_partialSize = high_order_sub_array<const T,ranks...>::size();

		return *(&m_ref + i_index * s_partialSize);
	}
	else
	{
		throw bad_access_exception{ "Index out of bounds" };
	}
}
template<typename T,size_t rank,size_t ... ranks>
constexpr size_t high_order_sub_array<T,rank,ranks...>::size()
{
    return rank * high_order_sub_array<T,ranks...>::size();
}

template<typename T>
high_order_sub_array<T>::high_order_sub_array(T& i_ref)
: m_ref(i_ref)
{
}
template<typename T>
template<typename TT>
high_order_sub_array<T>& high_order_sub_array<T>::operator=(TT&& i_value)
{
	m_ref = std::forward<TT>(i_value);

	return *this;
}
template<typename T>
high_order_sub_array<T>::operator T&()
{
	return m_ref;
}
template<typename T>
high_order_sub_array<T>::operator const T&() const
{
	return m_ref;
}
template<typename T>
constexpr size_t high_order_sub_array<T>::size()
{
    return 1;
}

}

template<typename T,size_t rank,size_t ... ranks>
T& high_order_array<T,rank,ranks...>::storage::operator[](size_t i_index)
{
	return m_data[i_index];
}
template<typename T,size_t rank,size_t ... ranks>
const T& high_order_array<T,rank,ranks...>::storage::operator[](size_t i_index) const
{
	return m_data[i_index];
}

template<typename T,size_t rank,size_t ... ranks>
TEMPLATE(typename ... Args)
REQUIRED(IS_NUMBER_OF_ARGS(num_places,Args...),IS_CONSTRUCTIBLE(T,Args)...)
constexpr high_order_array<T,rank,ranks...>::high_order_array(Args&& ... i_args)
: m_data({ i_args... })
{
}
template<typename T,size_t rank,size_t ... ranks>
TEMPLATE(typename TT)
REQUIRED(IS_CONSTRUCTIBLE(T,TT))
constexpr high_order_array<T,rank,ranks...>::high_order_array(const high_order_array<TT,rank,ranks...>& other)
{
	for(size_t index = 0; index < s_totalSize; ++index)
	{
		m_data[index] = other.m_data[index];
	}
}
template<typename T,size_t rank,size_t ... ranks>
detail::high_order_sub_array<T,ranks...> high_order_array<T,rank,ranks...>::operator[](size_t i_index)
{
	if(i_index < rank)
	{
		static const size_t s_partialSize = detail::high_order_sub_array<T,ranks...>::size();

		return m_data[i_index * s_partialSize];
	}
	else
	{
		throw bad_access_exception{ "Index out of bounds" };
	}
}
template<typename T,size_t rank,size_t ... ranks>
constexpr detail::high_order_sub_array<const T,ranks...> high_order_array<T,rank,ranks...>::operator[](size_t i_index) const
{
	if(i_index < rank)
	{
		return m_data[i_index * detail::high_order_sub_array<const T,ranks...>::size()];
	}
	else
	{
		throw bad_access_exception{ "Index out of bounds" };
	}
}
template<typename T,size_t rank,size_t ... ranks>
typename high_order_array<T,rank,ranks...>::reference high_order_array<T,rank,ranks...>::at(const high_order_array<size_t,s_numRanks>& i_indexs)
{
	static const size_t s_ranks[s_numRanks] = { rank, ranks... };

	size_t linearizedIndex = i_indexs[0];
	size_t accProdRank = s_ranks[0];
	for(size_t index=1;index < s_numRanks;++index)
	{
		linearizedIndex += i_indexs[index] * accProdRank;
		accProdRank *= s_ranks[index];
	}

	return m_data[linearizedIndex];
}
template<typename T,size_t rank,size_t ... ranks>
constexpr typename high_order_array<T,rank,ranks...>::const_reference high_order_array<T,rank,ranks...>::at(const high_order_array<size_t,s_numRanks>& i_indexs) const
{
	const size_t s_ranks[s_numRanks] = { rank, ranks... };

	size_t linearizedIndex = i_indexs[0];
	size_t accProdRank = s_ranks[0];
	for(size_t index = 1; index < s_numRanks; ++index)
	{
		linearizedIndex += i_indexs[index] * accProdRank;
		accProdRank *= s_ranks[index];
	}

	return m_data[linearizedIndex];
}
template<typename T,size_t rank,size_t ... ranks>
constexpr typename high_order_array<T,rank,ranks...>::reference high_order_array<T,rank,ranks...>::at(size_t i_index)
{
	return m_data[i_index];
}
template<typename T,size_t rank,size_t ... ranks>
constexpr typename high_order_array<T,rank,ranks...>::const_reference high_order_array<T,rank,ranks...>::at(size_t i_index) const
{
	return m_data[i_index];
}
template<typename T,size_t rank,size_t ... ranks>
high_order_array<T,rank,ranks...>& high_order_array<T,rank,ranks...>::operator=(const high_order_array<T,rank,ranks...>& other)
{
	if constexpr(std::is_trivially_copyable<T>::value)
	{
		memcpy(m_data,other.m_data,s_totalSize * sizeof(T));
	}
	else
	{
		for(size_t index = 0; index < s_totalSize; ++index)
		{
			m_data[index] = other.m_data[index];
		}
	}

	return *this;
}
template<typename T,size_t rank,size_t ... ranks>
template<typename TT>
high_order_array<T,rank,ranks...>& high_order_array<T,rank,ranks...>::operator=(const high_order_array<TT,rank,ranks...>& other)
{
	for(size_t index = 0; index < s_totalSize; ++index)
	{
		m_data[index] = other.m_data[index];
	}

	return *this;
}
template<typename T,size_t rank,size_t ... ranks>
constexpr size_t high_order_array<T,rank,ranks...>::size() const
{
	return s_totalSize;
}
template<typename T,size_t rank,size_t ... ranks>
constexpr bool high_order_array<T,rank,ranks...>::empty() const
{
	return false;
}

}
