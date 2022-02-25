#pragma once

#include "ddk_template_helper.h"
#include "ddk_system_allocator.h"
#include "ddk_none.h"

#define define_place_arg(_INDEX) \
static const ddk::mpl::place_holder<_INDEX> arg_##_INDEX;

namespace ddk
{

template<typename,typename = system_allocator>
class function;

namespace mpl
{

template<size_t Index>
struct place_holder
{
};

template<typename>
struct is_place_holder;

template<typename T>
struct is_place_holder
{
    static const bool value = false;
};
template<size_t Index>
struct is_place_holder<place_holder<Index>>
{
    static const bool value = true;
};

template<size_t currIndex,typename>
struct pos_place_holder;

template<size_t currIndex>
struct pos_place_holder<currIndex,type_pack<>>
{
    typedef sequence<> type;
};

template<size_t currIndex,typename Arg, typename ... Args>
struct pos_place_holder<currIndex,type_pack<Arg,Args...>>
{
private:
    typedef typename pos_place_holder<currIndex+1,type_pack<Args...>>::type next_seq;
    typedef typename std::remove_const<typename std::remove_reference<Arg>::type>::type raw_arg;

public:
    typedef typename static_if<is_place_holder<raw_arg>::value,typename merge_sequence<sequence<currIndex>,next_seq>::type,next_seq>::type type;
};

template<typename>
struct sequence_place_holder;

template<>
struct sequence_place_holder<type_pack<>>
{
    typedef sequence<> type;
};

template<typename Arg, typename ... Args>
struct sequence_place_holder<type_pack<Arg,Args...>>
{
private:
    template<typename T>
    struct arg_sequence
    {
        typedef sequence<> type;
    };
    template<size_t Index>
    struct arg_sequence<place_holder<Index>>
    {
        typedef sequence<Index> type;
    };

    typedef typename std::remove_const<typename std::remove_reference<Arg>::type>::type raw_arg;
    typedef typename arg_sequence<raw_arg>::type curr_sequence;
    typedef typename sequence_place_holder<type_pack<Args...>>::type next_sequence;

public:
    typedef typename merge_sequence<curr_sequence,next_sequence>::type type;
};

template<typename>
struct aqcuire_callable_return_type;

template<typename Return, typename ... Args>
struct aqcuire_callable_return_type<Return(*)(Args...)>
{
	typedef Return type;
};
template<typename Return, typename T, typename ... Args>
struct aqcuire_callable_return_type<Return(T::*)(Args...) const>
{
	typedef Return type;
};
template<typename Return,typename T,typename ... Args>
struct aqcuire_callable_return_type<Return(T::*)(Args...)>
{
	typedef Return type;
};
template<typename Functor>
struct aqcuire_callable_return_type
{
private:
    template<typename T, typename TT = decltype(&Functor::operator())>
    static typename aqcuire_callable_return_type<TT>::type resolve(T&);
    template<typename T>
    static typename T::return_type resolve(const T&, ...);
    static detail::none_t resolve(...);

public:
    typedef decltype(resolve(std::declval<Functor&>())) type;
};

template<typename>
struct aqcuire_callable_args_type;

template<typename Return,typename ... Args>
struct aqcuire_callable_args_type<Return(*)(Args...)>
{
    typedef type_pack<Args...> type;
};
template<typename Return,typename T,typename ... Args>
struct aqcuire_callable_args_type<Return(T::*)(Args...) const>
{
    typedef type_pack<Args...> type;
};
template<typename Return,typename T,typename ... Args>
struct aqcuire_callable_args_type<Return(T::*)(Args...)>
{
    typedef type_pack<Args...> type;
};
template<typename Functor>
struct aqcuire_callable_args_type
{
private:
    template<typename T,typename TT = decltype(&T::operator())>
    static typename aqcuire_callable_args_type<TT>::type resolve(T&);
    template<typename T>
    static typename T::args_type resolve(const T&,...);
    static detail::none_t resolve(...);

public:
    typedef decltype(resolve(std::declval<Functor&>())) type;
};

std::false_type _is_function(...);
template<typename T>
inline constexpr bool is_function = decltype(_is_function(std::declval<typename std::remove_reference<T>::type*>()))::value;

template<typename T, typename ... Args>
struct is_valid_functor
{
private:
    template<typename TT>
	static std::true_type _resolve(TT&, typename std::add_pointer<decltype(std::declval<TT>().operator()(std::declval<Args>() ...))>::type);
    template<typename TT>
	static std::false_type _resolve(TT&, ...);

    template<typename TT>
	static std::true_type resolve(TT&, decltype(&TT::operator()));
    template<typename TT, typename = typename TT::callable_tag>
	static std::true_type resolve(TT&, ...);
	static decltype(_resolve(std::declval<T&>(),nullptr)) resolve(...);

public:
    typedef typename static_if<is_function<T>,std::true_type,decltype(resolve(std::declval<T&>(),nullptr))>::type type;
    static const bool value = type::value;
};

}

define_place_arg(0)
define_place_arg(1)
define_place_arg(2)
define_place_arg(3)
define_place_arg(4)
define_place_arg(5)
define_place_arg(6)
define_place_arg(7)
define_place_arg(8)
define_place_arg(9)

}
