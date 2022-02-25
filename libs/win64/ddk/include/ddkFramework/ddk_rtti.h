#pragma once

#include "ddk_rtti_defs.h"
#include "ddk_default_values.h"
#include "ddk_concepts.h"
#include "ddk_rtti_concepts.h"
#include "ddk_static_counter.h"
#include "ddk_macros.h"
#include "ddk_visitable_type.h"

#define _PUBLISH_RTTI_INHERITANCE(_Type_Name,_Type_Interface) \
    ; typename decltype(__get_inherited_type_list(std::declval<ddk::rtti::detail::static_typed_number<decltype(ddk::mpl::resolve_holder<_Type_Interface>()),ddk::static_counter<decltype(ddk::mpl::resolve_holder<_Type_Interface>())>::get_curr_count()>>()))::add<decltype(ddk::mpl::resolve_holder<_Type_Name>())>::type __get_inherited_type_list(const ddk::rtti::detail::static_typed_number<decltype(ddk::mpl::resolve_holder<_Type_Interface>()),ddk::static_counter<decltype(ddk::mpl::resolve_holder<_Type_Interface>())>::get_next_count()>&);

#define PUBLISH_RTTI_INHERITANCE(_Type_Name,...) \
    FOREACH_ARG(_PUBLISH_RTTI_INHERITANCE,_Type_Name,__VA_ARGS__)

namespace ddk
{
namespace rtti
{
namespace detail
{

template<typename T, size_t Number>
struct static_typed_number
{
};
template<typename T>
struct static_typed_number<T,0>
{
    friend inline mpl::type_pack<> __get_inherited_type_list(const static_typed_number&);
};

template<typename>
struct get_inherited_type_list;

template<typename T>
struct get_inherited_type_list
{
private:
	static const size_t s_numberOfInheritedTypes = ddk::static_counter<mpl::class_holder<T>>::get_curr_count();
	typedef decltype(__get_inherited_type_list(std::declval<static_typed_number<mpl::class_holder<T>,s_numberOfInheritedTypes>>())) unresolved_type;

    template<typename ... TTT>
    static mpl::type_pack<typename TTT::type ...> resolve_types(const mpl::type_pack<TTT...>&);

public:
    template<typename ... TTT>
    static inline bool initialize_visitable_type(const mpl::type_pack<TTT...>&)
    {
        return (ddk::visitable_type<TTT,T>::s_initialized && ...);
    }

    typedef decltype(resolve_types(std::declval<unresolved_type>())) type;
};

template<template<typename...> typename T, typename ... TT>
struct get_inherited_type_list<T<TT...>>
{
private:
	static const size_t s_numberOfInheritedTypes = ddk::static_counter<mpl::template_class_holder<T>>::get_curr_count();
	typedef decltype(__get_inherited_type_list(std::declval<static_typed_number<mpl::template_class_holder<T>,s_numberOfInheritedTypes>>())) unresolved_type;
    template<typename,bool>
    struct resolved_type;
    template<typename TTT>
    struct resolved_type<TTT,false>
    {
        typedef typename TTT::type type;
    };
    template<typename TTT>
    struct resolved_type<TTT,true>
    {
        typedef typename TTT::template type<TT...> type;
    };

    template<typename ... TTT>
    static mpl::type_pack<typename resolved_type<TTT,mpl::is_templated_class_holder_v<TTT>>::type ...> resolve_types(const mpl::type_pack<TTT...>&);

public:

    template<typename ... TTT>
    static inline bool initialize_visitable_type(const mpl::type_pack<TTT...>&)
    {
        return (ddk::visitable_type<TTT,T<TT...>>::s_initialized && ...);
    }

    typedef decltype(resolve_types(std::declval<unresolved_type>())) type;
};

}

template<typename T>
using _inherited_type_list = typename detail::get_inherited_type_list<T>::type;
template<typename T>
using inherited_type_list = typename _inherited_type_list<T>::template drop_if<mpl::not_predicate<std::is_base_of,T>::template type>::type;
template<typename T>
static const bool inherited_type_expansion = detail::get_inherited_type_list<T>::template initialize_visitable_type(inherited_type_list<T>{});

}

}
