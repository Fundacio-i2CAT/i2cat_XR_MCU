#pragma once

#include "ddk_tuple.h"

namespace ddk
{

template<typename ... Types>
class function_arguments : public tuple<Types...>
{
public:
    using tuple<Types...>::tuple;
};

template<typename Arg, typename ... Args>
inline function_arguments<Arg,Args...> make_function_arguments(Arg&& i_arg, Args&& ... i_args);

template<typename>
struct _is_function_argument;

template<typename T>
struct _is_function_argument
{
    static const bool value = false;
};

template<typename ... Types>
struct _is_function_argument<function_arguments<Types...>>
{
    static const bool value = true;
};

template<typename T>
struct is_function_argument
{
private:
    typedef typename std::remove_reference<typename std::remove_const<T>::type>::type raw_type;

public:
    static const bool value = _is_function_argument<raw_type>::value;
};

}

#include "ddk_function_arguments.inl"