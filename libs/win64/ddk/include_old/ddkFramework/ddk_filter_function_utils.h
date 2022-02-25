#pragma once

namespace ddk
{

//no allocator specified, no args specified
template<typename Object, typename ... Types>
inline filter_function<bool(Types...)> make_filter_function(Object* i_object, bool(Object::*i_funcPtr)(Types...));
template<typename Object, typename ... Types>
inline filter_function<bool(Types...)> make_filter_function(const Object* i_object, bool(Object::*i_funcPtr)(Types...)const);
template<typename ... Types>
inline filter_function<bool(Types...)> make_filter_function(bool(*i_funcPtr)(Types...));
template<typename Functor>
inline resolved_filter_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type> make_filter_function(Functor&&);

//allocator specified, no args specified
template<typename Object, typename ... Types, typename Allocator>
inline filter_function<bool(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_filter_function(Object* i_object, bool(Object::*i_funcPtr)(Types...), const Allocator& i_allocator);
template<typename Object, typename ... Types, typename Allocator>
inline filter_function<bool(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_filter_function(const Object* i_object, bool(Object::*i_funcPtr)(Types...)const, const Allocator& i_allocator);
template<typename ... Types, typename Allocator>
inline filter_function<bool(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_filter_function(bool(*i_funcPtr)(Types...), const Allocator& i_allocator);
template<typename Functor, typename Allocator>
inline resolved_filter_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,Allocator> make_filter_function(Functor&&, const Allocator& i_allocator, typename std::enable_if<mpl::is_allocator<Allocator>::value>::type* = nullptr);

}

#include "ddk_filter_function_utils.inl"
