
namespace ddk
{

template<typename Object, typename ... Types>
filter_function<bool(Types...)> make_filter_function(Object* i_object, bool(Object::*i_funcPtr)(Types...))
{
    return filter_function<bool(Types...)>(i_object, i_funcPtr);
}
template<typename Object, typename ... Types>
filter_function<bool(Types...)> make_filter_function(const Object* i_object, bool(Object::*i_funcPtr)(Types...)const)
{
    return filter_function<bool(Types...)>(i_object,i_funcPtr);
}
template<typename ... Types>
filter_function<bool(Types...)> make_filter_function(bool(*i_funcPtr)(Types...))
{
    return filter_function<bool(Types...)>(i_funcPtr);
}
template<typename Functor>
resolved_filter_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type> make_filter_function(Functor&& i_functor)
{
    typedef resolved_filter_callable<Functor> function_type;

    return function_type(i_functor);
}
template<typename Object, typename ... Types, typename Allocator>
filter_function<bool(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_filter_function(Object* i_object, bool(Object::*i_funcPtr)(Types...), const Allocator& i_allocator)
{
    return filter_function<bool(Types...),Allocator>(i_object, i_funcPtr);
}
template<typename Object, typename ... Types, typename Allocator>
filter_function<bool(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_filter_function(const Object* i_object, bool(Object::*i_funcPtr)(Types...)const, const Allocator& i_allocator)
{
    return filter_function<bool(Types...),Allocator>(i_object, reinterpret_cast<bool(Object::*)(Types...)>(i_funcPtr));
}
template<typename ... Types, typename Allocator>
filter_function<bool(Types...),typename std::enable_if<mpl::is_allocator<Allocator>::value,Allocator>::type> make_filter_function(bool(*i_funcPtr)(Types...), const Allocator& i_allocator)
{
    return filter_function<bool(Types...),Allocator>(i_funcPtr);
}
template<typename Functor, typename Allocator>
resolved_filter_callable<typename std::enable_if<std::is_class<Functor>::value,Functor>::type,Allocator> make_filter_function(Functor&& i_functor, const Allocator& i_allocator, typename std::enable_if<mpl::is_allocator<Allocator>::value>::type*)
{
    typedef resolved_filter_callable<Functor,Allocator> function_type;

    return function_type(i_functor,i_allocator);
}

}
