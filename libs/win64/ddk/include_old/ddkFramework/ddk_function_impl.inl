
#include "ddk_reference_wrapper.h"
#include "ddk_function_exceptions.h"
#include "ddk_allocator_exceptions.h"

namespace ddk
{
namespace detail
{

template<typename Return, typename ... Types>
template<size_t ... specIndexs, size_t ... notSpecIndexs>
template<typename ... Args>
function_impl_base<Return,mpl::type_pack<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::specialized_impl(const distributed_reference_wrapper<const function_impl_base<Return,mpl::type_pack<Types...>>>& i_object, tuple<Args...>&& i_args)
: m_object(i_object)
, m_specArgs(std::move(i_args).template extract<specIndexs>() ...)
{
}
template<typename Return, typename ... Types>
template<size_t ... specIndexs, size_t ... notSpecIndexs>
Return function_impl_base<Return,mpl::type_pack<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::operator()(forwarded_arg<typename mpl::nth_type_of<notSpecIndexs,Types...>::type> ... i_args) const
{
    typedef typename mpl::merge_sequence<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::type total_indexs;
    typedef typename mpl::inverse_sequence<total_indexs>::type inverse_total_indexs;

    if constexpr (std::is_same<Return,void>::value)
    {
        m_object->apply(merge_args<typename mpl::nth_type_of<specIndexs,Types...>::type ...,forwarded_arg<typename mpl::nth_type_of<notSpecIndexs,Types...>::type> ...>(total_indexs{},inverse_total_indexs{},m_specArgs,std::forward<forwarded_arg<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>>(i_args) ...));
    }
    else
    {
        return m_object->apply(merge_args<typename mpl::nth_type_of<specIndexs,Types...>::type ...,forwarded_arg<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>...>(total_indexs{},inverse_total_indexs{},m_specArgs,std::forward<forwarded_arg<typename mpl::nth_type_of<notSpecIndexs,Types...>::type>>(i_args) ...));
    }
}
template<typename Return, typename ... Types>
template<size_t ... specIndexs, size_t ... notSpecIndexs>
Return function_impl_base<Return,mpl::type_pack<Types...>>::specialized_impl<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::apply(const vars_tuple& i_tuple) const
{
    typedef typename mpl::merge_sequence<mpl::sequence<specIndexs...>,mpl::sequence<notSpecIndexs...>>::type total_indexs;
    typedef typename mpl::inverse_sequence<total_indexs>::type inverse_total_indexs;

    if constexpr (std::is_same<Return,void>::value)
    {
        m_object->apply(merge<typename mpl::nth_type_of<specIndexs,Types...>::type ...,typename mpl::nth_type_of<notSpecIndexs,forwarded_arg<Types>...>::type ...>(total_indexs{},inverse_total_indexs{},m_specArgs,i_tuple));
    }
    else
    {
        return m_object->apply(merge<typename mpl::nth_type_of<specIndexs,Types...>::type ...,typename mpl::nth_type_of<notSpecIndexs,forwarded_arg<Types>...>::type ...>(total_indexs{},inverse_total_indexs{},m_specArgs,i_tuple));
    }
}

template<typename Return, typename ... Types>
template<typename Allocator, typename ... Args>
function_impl_base_const_dist_ref<Return,unresolved_types<mpl::type_pack<Args...>,Types...>> function_impl_base<Return,mpl::type_pack<Types...>>::specialize(const Allocator& i_allocator, Args&& ... args) const
{
    typedef typename mpl::pos_place_holder<0,mpl::type_pack<Args...>>::type not_spec_indexs;
    typedef typename mpl::dual_sequence<not_spec_indexs>::template at<0,s_numTypes>::type spec_sequence;
    typedef typename not_spec_indexs::template at<typename mpl::sequence_place_holder<mpl::type_pack<Args...>>::type>::type not_spec_sequence;
    typedef specialized_impl<spec_sequence,not_spec_sequence> spec_func_type;

	std::pair<resource_deleter_const_lent_ref,void*> allocCtxt = i_allocator.allocate(sizeof(spec_func_type));

	spec_func_type* newFuncImpl = new(allocCtxt.second) spec_func_type(this->ref_from_this(),ddk::make_tuple(std::forward<Args>(args)...));

    return as_distributed_reference(newFuncImpl,allocCtxt.first);
}

template<typename ObjectType, typename Return, typename ... Types>
constexpr relative_function_impl<ObjectType,Return,Types...>::relative_function_impl(ObjectType* i_object, FuncPointerType i_funcPointer)
: m_object(i_object)
, m_funcPointer(i_funcPointer)
{
}
template<typename ObjectType,typename Return,typename ... Types>
constexpr relative_function_impl<ObjectType,Return,Types...>::relative_function_impl(relative_function_impl&& other)
: m_object(other.m_object)
, m_funcPointer(other.m_funcPointer)
{
	other.m_object = nullptr;
	other.m_funcPointer = nullptr;
}
template<typename ObjectType,typename Return,typename ... Types>
inline Return relative_function_impl<ObjectType,Return,Types...>::inline_eval(forwarded_arg<Types> ... args) const
{
	if constexpr(std::is_same<Return,void>::value)
	{
		(m_object->*m_funcPointer)(std::forward<forwarded_arg<Types>>(args)...);
	}
	else
	{
		return (m_object->*m_funcPointer)(std::forward<forwarded_arg<Types>>(args)...);
	}
}
template<typename ObjectType, typename Return, typename ... Types>
Return relative_function_impl<ObjectType,Return,Types...>::operator()(forwarded_arg<Types> ... args) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        (m_object->*m_funcPointer)(std::forward<forwarded_arg<Types>>(args)...);
    }
    else
    {
        return (m_object->*m_funcPointer)(std::forward<forwarded_arg<Types>>(args)...);
    }
}
template<typename ObjectType, typename Return, typename ... Types>
Return relative_function_impl<ObjectType,Return,Types...>::apply(const tuple_args& i_tuple) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type types_sequence;

    if constexpr (std::is_same<Return,void>::value)
    {
        apply(types_sequence{},i_tuple);
    }
    else
    {
        return apply(types_sequence{},i_tuple);
    }
}
template<typename ObjectType, typename Return, typename ... Types>
template<size_t ... Indexs>
Return relative_function_impl<ObjectType,Return,Types...>::apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        (m_object->*m_funcPointer)(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
    else
    {
        return (m_object->*m_funcPointer)(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
}

template<typename Return, typename ... Types>
constexpr free_function_impl<Return,Types...>::free_function_impl(FuncPointerType i_funcPointer)
: m_funcPointer(i_funcPointer)
{
}
template<typename Return,typename ... Types>
inline Return free_function_impl<Return,Types...>::inline_eval(forwarded_arg<Types> ... args) const
{
	if constexpr(std::is_same<Return,void>::value)
	{
		(*m_funcPointer)(std::forward<forwarded_arg<Types>>(args)...);
	}
	else
	{
		return (*m_funcPointer)(std::forward<forwarded_arg<Types>>(args)...);
	}
}
template<typename Return, typename ... Types>
Return free_function_impl<Return,Types...>::operator()(forwarded_arg<Types> ... args) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        (*m_funcPointer)(std::forward<forwarded_arg<Types>>(args)...);
    }
    else
    {
        return (*m_funcPointer)(std::forward<forwarded_arg<Types>>(args)...);
    }
}
template<typename Return, typename ... Types>
Return free_function_impl<Return,Types...>::apply(const tuple_args& i_tuple) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type types_sequence;

    if constexpr (std::is_same<Return,void>::value)
    {
        apply(types_sequence{},i_tuple);
    }
    else
    {
        return apply(types_sequence{},i_tuple);
    }
}
template<typename Return, typename ... Types>
template<size_t ... Indexs>
Return free_function_impl<Return,Types...>::apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        (*m_funcPointer)(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
    else
    {
        return (*m_funcPointer)(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
}

template<typename T, typename Return, typename ... Types>
constexpr aggregated_functor_impl<T,Return,Types...>::aggregated_functor_impl(const T& i_functor)
: m_functor(i_functor)
{
}
template<typename T,typename Return,typename ... Types>
constexpr aggregated_functor_impl<T,Return,Types...>::aggregated_functor_impl(T&& i_functor)
: m_functor(std::move(i_functor))
{
}
template<typename T,typename Return,typename ... Types>
inline Return aggregated_functor_impl<T,Return,Types...>::inline_eval(forwarded_arg<Types> ... args) const
{
	if constexpr(std::is_same<Return,void>::value)
	{
		m_functor(std::forward<decltype(args)>(args)...);
	}
	else
	{
		return m_functor(std::forward<decltype(args)>(args)...);
	}
}
template<typename T, typename Return, typename ... Types>
Return aggregated_functor_impl<T,Return,Types...>::operator()(forwarded_arg<Types> ... args) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        m_functor(std::forward<decltype(args)>(args)...);
    }
    else
    {
        return m_functor(std::forward<decltype(args)>(args)...);
    }
}
template<typename T, typename Return, typename ... Types>
Return aggregated_functor_impl<T,Return,Types...>::apply(const tuple_args& i_tuple) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type types_sequence;

    if constexpr (std::is_same<Return,void>::value)
    {
        apply(types_sequence{},i_tuple);
    }
    else
    {
        return apply(types_sequence{},i_tuple);
    }
}
template<typename T, typename Return, typename ... Types>
template<size_t ... Indexs>
Return aggregated_functor_impl<T,Return,Types...>::apply(const mpl::sequence<Indexs...>&, const tuple_args& i_tuple) const
{
    if constexpr (std::is_same<Return,void>::value)
    {
        m_functor(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
    else
    {
        return m_functor(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
}

template<typename Return,typename ... Types>
Return inherited_functor_impl<Return,Types...>::inline_eval(forwarded_arg<Types> ... args) const
{
    if constexpr(std::is_same<Return,void>::value)
    {
        this->operator()(std::forward<decltype(args)>(args)...);
    }
    else
    {
        return this->operator()(std::forward<decltype(args)>(args)...);
    }
}
template<typename Return,typename ... Types>
Return inherited_functor_impl<Return,Types...>::apply(const tuple_args& i_tuple) const
{
    typedef typename mpl::make_sequence<0,s_numTypes>::type types_sequence;

    if constexpr(std::is_same<Return,void>::value)
    {
        apply(types_sequence{},i_tuple);
    }
    else
    {
        return apply(types_sequence{},i_tuple);
    }
}
template<typename Return,typename ... Types>
template<size_t ... Indexs>
Return inherited_functor_impl<Return,Types...>::apply(const mpl::sequence<Indexs...>&,const tuple_args& i_tuple) const
{
    if constexpr(std::is_same<Return,void>::value)
    {
        this->operator()(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
    else
    {
        return this->operator()(const_cast<tuple_args&>(i_tuple).template get<Indexs>()...);
    }
}

}
}
