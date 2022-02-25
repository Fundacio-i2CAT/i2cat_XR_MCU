#pragma once

#include "ddk_template_helper.h"
#include <type_traits>

#define IS_INHERITED_VALUE(_TYPE) \
	typename std::enable_if<ddk::concepts::is_inherited_value_v<_TYPE>>::type

#define IS_NOT_INHERITED_VALUE(_TYPE) \
	typename std::enable_if<ddk::concepts::is_inherited_value_v<_TYPE> == false>::type

#define IS_DYNAMIC_VISITOR(_TYPE) \
	typename std::enable_if<ddk::concepts::is_dynamic_visitor_v<_TYPE>>::type

#define IS_BASE_OF_DYNAMIC_VISITOR(_TYPE) \
	typename std::enable_if<ddk::concepts::is_base_of_dynamic_visitor_v<_TYPE>>::type

namespace ddk
{

template<typename>
class dynamic_visitor;

namespace concepts
{

template<typename T>
struct is_inherited_value
{
private:
    template<typename TT>
    static std::true_type resolve(const inherited_value<TT>&);
    template<typename ... TT>
    static std::false_type resolve(const TT& ...);

public:
    static const bool value = decltype(resolve(std::declval<T>()))::value;
};

template<typename T>
inline constexpr bool is_inherited_value_v = is_inherited_value<T>::value;

template<typename T>
struct is_dynamic_visitor;

template<typename T>
struct is_dynamic_visitor<dynamic_visitor<T>> : public std::true_type
{
};
template<typename T>
struct is_dynamic_visitor : public std::false_type
{
};

template<typename T>
inline constexpr bool is_dynamic_visitor_v = is_dynamic_visitor<T>::value;

template<typename T>
struct is_base_of_dynamic_visitor
{
private:
    template<typename TT>
    static std::is_base_of<dynamic_visitor<typename TT::type_interface>,TT> resolve(const TT&);
    static std::false_type resolve(...);

public:
    static const bool value = decltype(resolve(std::declval<T>()))::value;
};

template<typename T>
inline constexpr bool is_base_of_dynamic_visitor_v = is_base_of_dynamic_visitor<T>::value;

}
}
