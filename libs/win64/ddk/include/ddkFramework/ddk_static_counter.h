#pragma once

#if defined(WIN32)

#define INITIAL_VALUE 1

#else

#define INITIAL_VALUE 0

#endif


namespace ddk
{
namespace detail
{

template<typename T,size_t N>
struct counter_tag
{
	friend constexpr size_t ___counter_index(const counter_tag<T,N>&);
};

template<typename T,size_t N>
struct producer
{
	friend constexpr size_t ___counter_index(const counter_tag<T,N>&)
	{
		return N;
	}

	static constexpr size_t value = ___counter_index(counter_tag<T,N>{});
};

template<typename T,size_t N,size_t = producer<T,N>::value, typename ... TT>
size_t constexpr consumer(float,const counter_tag<T,N>&,TT...)
{
	return N;
}

template<typename T,size_t N,size_t V = ___counter_index(counter_tag<T,N>{}), size_t R = consumer<T>(0,counter_tag<T,N + 1> {})>
size_t constexpr consumer(int,const counter_tag<T,N>&)
{
	return R;
}

template<typename T,size_t N, size_t V = 100>
size_t constexpr curr_tag(float,const counter_tag<T,N>&, ...)
{
	return N-1;
}

template<typename T,size_t N,size_t V = ___counter_index(counter_tag<T,N>{}), size_t R  = curr_tag<T>(0,counter_tag<T,N + 1> {})>
size_t constexpr curr_tag(int,const counter_tag<T,N>&)
{
	return R;
}

template<typename T, size_t R = consumer<T>(0,counter_tag<T,INITIAL_VALUE> {})>
size_t constexpr next_value()
{
	return R;
}

template<typename T, size_t R = curr_tag<T>(0,counter_tag<T,INITIAL_VALUE> {})>
size_t constexpr curr_value()
{
	return R;
}

template<typename T, size_t R>
struct query_value_struct
{
    static constexpr size_t value()
    {
        return R;
    }
};

}

template<typename T>
struct static_counter
{
    template<size_t R = detail::next_value<T>()>
	static constexpr size_t get_next_count()
	{
		return detail::query_value_struct<T,R>::value();
	}
    template<size_t R = detail::curr_value<T>()>
	static constexpr size_t get_curr_count()
	{
		return detail::query_value_struct<T,R>::value();
	}
};

}
