#pragma once

#include "ddk_shared_reference_wrapper.h"
#include "ddk_unique_reference_wrapper.h"

#define IS_SHARED_REFERENCE(_TYPE) \
	typename std::enable_if<ddk::concepts::is_shared_reference_v<_TYPE>>::type*

#define IS_NOT_SHARED_REFERENCE(_TYPE) \
	typename std::enable_if<ddk::concepts::is_shared_reference_v<_TYPE> == false>::type*

#define IS_SHARED_POINTER(_TYPE) \
	typename std::enable_if<ddk::concepts::is_shared_pointer_v<_TYPE>>::type*

#define IS_NOT_SHARED_POINTER(_TYPE) \
		typename std::enable_if<ddk::concepts::is_shared_pointer_v<_TYPE> == false>::type*

#define IS_UNIQUE_REFERENCE(_TYPE) \
	typename std::enable_if<ddk::concepts::is_unique_reference_v<_TYPE>>::type*

#define IS_NOT_UNIQUE_REFERENCE(_TYPE) \
	typename std::enable_if<ddk::concepts::is_unique_reference_v<_TYPE> == false>::type*

#define IS_UNIQUE_POINTER(_TYPE) \
	typename std::enable_if<ddk::concepts::is_unique_pointer_v<_TYPE>>::type*

#define IS_NOT_UNIQUE_POINTER(_TYPE) \
	typename std::enable_if<ddk::concepts::is_unique_pointer_v<_TYPE> == false>::type*


namespace ddk
{
namespace concepts
{

template<typename>
struct is_shared_reference;

template<typename T>
struct is_shared_reference<shared_reference_wrapper<T>>
{
	static const bool value = true;
};
template<typename>
struct is_shared_reference
{
	static const bool value = false;
};

template<typename T>
constexpr bool is_shared_reference_v = is_shared_reference<T>::value;

template<typename>
struct is_shared_pointer;

template<typename T>
struct is_shared_pointer<shared_pointer_wrapper<T>>
{
	static const bool value = true;
};
template<typename>
struct is_shared_pointer
{
	static const bool value = false;
};

template<typename T>
constexpr bool is_shared_pointer_v = is_shared_pointer<T>::value;

template<typename>
struct is_unique_reference;

template<typename T>
struct is_unique_reference<shared_reference_wrapper<T>>
{
	static const bool value = true;
};
template<typename>
struct is_unique_reference
{
	static const bool value = false;
};

template<typename T>
constexpr bool is_unique_reference_v = is_unique_reference<T>::value;

template<typename>
struct is_unique_pointer;

template<typename T>
struct is_unique_pointer<unique_pointer_wrapper<T>>
{
	static const bool value = true;
};
template<typename>
struct is_unique_pointer
{
	static const bool value = false;
};

template<typename T>
constexpr bool is_unique_pointer_v = is_unique_pointer<T>::value;

}
}