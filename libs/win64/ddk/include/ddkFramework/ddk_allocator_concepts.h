#pragma once

#include "ddk_type_concepts.h"

#define IS_ALLOCATOR_COND(_TYPE) \
	ddk::concepts::is_allocator<_TYPE>::value

#define IS_ALLOCATOR(_TYPE) \
	typename std::enable_if<IS_ALLOCATOR_COND(_TYPE)>::type*

#define IS_NOT_ALLOCATOR_COND(_TYPE) \
	(ddk::concepts::is_allocator<_TYPE>::value==false)

#define IS_NOT_ALLOCATOR(_TYPE) \
	typename std::enable_if<IS_NOT_ALLOCATOR_COND(_TYPE)>::type*

namespace ddk
{
namespace concepts
{

template<typename T>
struct is_allocator
{
private:
	template<typename TT, typename = decltype(std::declval<TT>().allocate(1)), typename = decltype(std::declval<TT>().deallocate(nullptr))>
	static std::true_type test(const TT&);
	template<typename ... TT>
	static std::false_type test(const TT& ...);

public:
	static const bool value = decltype(test(std::declval<T>()))::value;
};

}
}