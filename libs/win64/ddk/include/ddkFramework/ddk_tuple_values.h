#pragma once

#include "ddk_tuple.h"

namespace ddk
{

template<typename ... Types>
class values_tuple : public tuple<Types...>
{
public:
    using tuple<Types...>::tuple;
};

template<typename ... Args>
inline values_tuple<Args...> make_values_tuple(Args&& ... i_args);

template<typename>
struct _is_values_tuple;

template<typename T>
struct _is_values_tuple
{
    static const bool value = false;
};

template<typename ... Types>
struct _is_values_tuple<values_tuple<Types...>>
{
    static const bool value = true;
};

template<typename T>
struct is_values_tuple
{
private:
    typedef typename std::remove_reference<typename std::remove_const<T>::type>::type raw_type;

public:
    static const bool value = _is_values_tuple<raw_type>::value;
};

}

#include "ddk_tuple_values.inl"
