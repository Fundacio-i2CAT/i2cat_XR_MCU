#pragma once

#include "ddk_function_template_helper.h"

#define IS_FUNCTION(_TYPE) \
	typename std::enable_if<ddk::mpl::is_function<_TYPE>>::type

#define IS_NOT_FUNCTION(_TYPE) \
	typename std::enable_if<ddk::mpl::is_function<_TYPE> == false>::type

#define IS_CALLABLE(_TYPE, ...) \
	typename std::enable_if<ddk::mpl::is_valid_functor<typename std::remove_reference<_TYPE>::type,##__VA_ARGS__>::value>::type

#define IS_CALLABLE_COND(_TYPE, ...) \
	ddk::mpl::is_valid_functor<typename std::remove_reference<_TYPE>::type,##__VA_ARGS__>::value

#define IS_NOT_CALLABLE(_TYPE, ...) \
	typename std::enable_if<ddk::mpl::is_valid_functor<typename std::remove_reference<_TYPE>::type,##__VA_ARGS__>::value==false>::type

#define IS_NOT_CALLABLE_COND(_TYPE, ...) \
	ddk::mpl::is_valid_functor<typename std::remove_reference<_TYPE>::type,##__VA_ARGS__>::value==false

#define IS_RETURN_TYPE_CONVERTIBLE_TO(_TYPE,_RETURN) \
	typename std::enable_if<std::is_convertible<typename ddk::mpl::aqcuire_callable_return_type<_TYPE>::type,_RETURN>::value>::type

#define IS_CALLABLE_NOT_FUNCTION(_TYPE,...) \
	IS_CALLABLE(_TYPE,##__VA_ARGS__),IS_NOT_FUNCTION(_TYPE)
