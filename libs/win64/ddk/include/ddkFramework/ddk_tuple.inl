
#include <memory>

namespace ddk
{
namespace detail
{

template<size_t Index,typename T>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
constexpr tuple_base<Index,T>::tuple_base(Args&& ... i_args)
: m_value(std::forward<Args>(i_args)...)
{
}
template<size_t Index,typename T>
constexpr typename tuple_base<Index,T>::reference tuple_base<Index,T>::get()
{
    return m_value.get();
}
template<size_t Index,typename T>
typename tuple_base<Index,T>::rreference tuple_base<Index,T>::extract()&&
{
    return m_value.extract();
}
template<size_t Index,typename T>
constexpr typename tuple_base<Index,T>::const_reference tuple_base<Index,T>::get() const
{
    return m_value.get();
}
template<size_t Index,typename T>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
bool tuple_base<Index,T>::set(Args&& ... i_args)
{
    m_value = T{ std::forward<Args>(i_args)... };

    return true;
}

template<typename Type>
TEMPLATE(size_t IIndex,typename Arg)
REQUIRED(IS_SAME_RANK(IIndex,0),IS_CONSTRUCTIBLE(Type,Arg))
constexpr tuple_impl<mpl::sequence<0>,Type>::tuple_impl(const mpl::sequence<IIndex>&,Arg&& i_arg)
: m_val(std::forward<Arg>(i_arg))
{
    static_assert(IIndex == 0,"Index out of bounds");
}
template<typename Type>
TEMPLATE(typename Arg)
REQUIRED(IS_CONSTRUCTIBLE(Type,Arg))
constexpr tuple_impl<mpl::sequence<0>,Type>::tuple_impl(Arg&& i_val)
: m_val(std::forward<Arg>(i_val))
{
}
template<typename Type>
template<typename TType>
constexpr tuple_impl<mpl::sequence<0>,Type>::tuple_impl(const tuple_impl<mpl::sequence<0>,TType>& other)
: m_val(other.m_val)
{
}
template<typename Type>
template<typename TType>
constexpr tuple_impl<mpl::sequence<0>,Type>::tuple_impl(tuple_impl<mpl::sequence<0>,TType>&& other)
: m_val(std::move(other.m_val))
{
}
template<typename Type>
template<size_t IIndex>
constexpr typename embedded_type<Type>::cref_type tuple_impl<mpl::sequence<0>,Type>::get() const
{
    return std::forward<typename embedded_type<Type>::cref_type>(m_val);
}
template<typename Type>
template<size_t IIndex>
constexpr typename embedded_type<Type>::ref_type tuple_impl<mpl::sequence<0>,Type>::get()
{
    return std::forward<typename embedded_type<Type>::ref_type>(m_val);
}
template<typename Type>
template<size_t IIndex>
typename embedded_type<Type>::rref_type tuple_impl<mpl::sequence<0>,Type>::extract()&&
{
    return std::forward<typename embedded_type<Type>::rref_type>(m_val);
}
template<typename Type>
template<size_t Index,typename Arg>
typename embedded_type<Type>::ref_type tuple_impl<mpl::sequence<0>,Type>::set(Arg&& i_arg)
{
    return (m_val = std::forward<Arg>(i_arg));
}
template<typename Type>
tuple_impl<mpl::sequence<0>,Type>* tuple_impl<mpl::sequence<0>,Type>::operator->()
{
    return this;
}
template<typename Type>
constexpr const tuple_impl<mpl::sequence<0>,Type>* tuple_impl<mpl::sequence<0>,Type>::operator->() const
{
    return this;
}
template<typename Type>
constexpr size_t tuple_impl<mpl::sequence<0>,Type>::size()
{
    return 1;
}
template<typename Type>
constexpr bool tuple_impl<mpl::sequence<0>,Type>::empty() const
{
    return false;
}

template<size_t ... Indexs,typename ... Types>
TEMPLATE(size_t ... IIndexs,typename ... TTypes)
REQUIRED(IS_CONSTRUCTIBLE(type_by_index<IIndexs>,TTypes)...)
constexpr tuple_impl<mpl::sequence<Indexs...>,Types...>::tuple_impl(const mpl::sequence<IIndexs...>&,TTypes&& ... i_args)
: tuple_base<IIndexs,type_by_index<IIndexs>>(std::forward<TTypes>(i_args)) ...
{
}
template<size_t ... Indexs,typename ... Types>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(Types,Args)...)
constexpr tuple_impl<mpl::sequence<Indexs...>,Types...>::tuple_impl(Args&& ... i_args)
: tuple_base<Indexs,Types>(std::forward<Args>(i_args)) ...
{
}
template<size_t ... Indexs,typename ... Types>
template<typename ... TTypes>
constexpr tuple_impl<mpl::sequence<Indexs...>,Types...>::tuple_impl(const tuple_impl<mpl::sequence<Indexs...>,TTypes...>& other)
: tuple_base<Indexs,Types>(std::forward<TTypes>(other.template get<Indexs>())) ...
{
}
template<size_t ... Indexs,typename ... Types>
template<typename ... TTypes>
constexpr tuple_impl<mpl::sequence<Indexs...>,Types...>::tuple_impl(tuple_impl<mpl::sequence<Indexs...>,TTypes...>&& other)
: tuple_base<Indexs,Types>(std::move(other).template extract<Indexs>()) ...
{
}
template<size_t ... Indexs,typename ... Types>
constexpr tuple_impl<mpl::sequence<Indexs...>,Types...>::tuple_impl(const tuple_impl& other)
: tuple_base<Indexs,Types>(other.template get<Indexs>()) ...
{
}
template<size_t ... Indexs,typename ... Types>
constexpr tuple_impl<mpl::sequence<Indexs...>,Types...>::tuple_impl(tuple_impl&& other)
: tuple_base<Indexs,Types>(std::move(other).template extract<Indexs>()) ...
{
}
template<size_t ... Indexs,typename ... Types>
template<size_t IIndex>
constexpr typename tuple_base_by_index<IIndex,Types...>::const_reference tuple_impl<mpl::sequence<Indexs...>,Types...>::get() const
{
    return static_cast<const tuple_base<IIndex,type_by_index<IIndex>>&>(*this).get();
}
template<size_t ... Indexs,typename ... Types>
template<size_t IIndex>
constexpr typename tuple_base_by_index<IIndex,Types...>::reference tuple_impl<mpl::sequence<Indexs...>,Types...>::get()
{
    return static_cast<tuple_base<IIndex,type_by_index<IIndex>>&>(*this).get();
}
template<size_t ... Indexs,typename ... Types>
template<size_t IIndex>
typename tuple_base_by_index<IIndex,Types...>::rreference tuple_impl<mpl::sequence<Indexs...>,Types...>::extract()&&
{
    return static_cast<tuple_base<IIndex,type_by_index<IIndex>>&&>(*this).extract();
}
template<size_t ... Indexs,typename ... Types>
template<size_t ... IIndexs,typename ... Args>
void tuple_impl<mpl::sequence<Indexs...>,Types...>::set(const mpl::sequence<IIndexs...>&,Args&& ... i_args)
{
    static_assert(mpl::get_num_ranks<IIndexs...>() == mpl::get_num_types<Args...>(),"Unconsistent provided arguments and sequence");
    static_assert(mpl::get_num_types<Types...>() == mpl::get_num_types<Args...>(),"Wrong number of arguments");

    (static_cast<tuple_base<IIndexs,type_by_index<IIndexs>>&>(*this).set(std::forward<Args>(i_args)) && ...);
}
template<size_t ... Indexs,typename ... Types>
template<size_t IIndex,typename Arg>
typename tuple_base_by_index<IIndex,Types...>::reference tuple_impl<mpl::sequence<Indexs...>,Types...>::set(Arg&& i_arg)
{
    static_cast<tuple_base<IIndex,type_by_index<IIndex>>&>(*this).set(std::forward<Arg>(i_arg));

    return static_cast<tuple_base<IIndex,type_by_index<IIndex>>&>(*this).get();
}
template<size_t ... Indexs,typename ... Types>
tuple_impl<mpl::sequence<Indexs...>,Types...>* tuple_impl<mpl::sequence<Indexs...>,Types...>::operator->()
{
    return this;
}
template<size_t ... Indexs,typename ... Types>
const tuple_impl<mpl::sequence<Indexs...>,Types...>* tuple_impl<mpl::sequence<Indexs...>,Types...>::operator->() const
{
    return this;
}
template<size_t ... Indexs,typename ... Types>
constexpr size_t tuple_impl<mpl::sequence<Indexs...>,Types...>::size()
{
    return mpl::num_types<Types...>;
}
template<size_t ... Indexs,typename ... Types>
constexpr bool tuple_impl<mpl::sequence<Indexs...>,Types...>::empty() const
{
    return false;
}

}

template<typename ... Types>
constexpr auto make_tuple(Types&& ... vals)
{
    return tuple<Types...>(std::forward<Types>(vals)...);
}
template<size_t ... Indexs, size_t ... IIndexs,typename ... Types>
constexpr auto make_indexed_tuple(const mpl::sequence<Indexs...>& i_inverseSeq, const mpl::sequence<IIndexs...>& i_seq,Types&& ... vals)
{
    return tuple<mpl::nth_type_of_t<Indexs,Types...>...>(i_seq,std::forward<Types>(vals)...);
}
template<size_t ... Indexs,typename ... Types>
constexpr auto make_indexed_tuple(const mpl::sequence<Indexs...>& i_seq, Types&& ... vals)
{
    return make_indexed_tuple(typename mpl::inverse_sequence<mpl::sequence<Indexs...>>::type{},i_seq,std::forward<Types>(vals)...);
}

template<typename ... FinalTypes,size_t ... FromIndexs,size_t ... ToIndexs,size_t ... IndexsA,typename ... TypesA,size_t ... IndexsB,typename ... TypesB>
constexpr tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge(const mpl::sequence<FromIndexs...>& i_srcSeq,const mpl::sequence<ToIndexs...>&,const mpl::sequence<IndexsA...>&,tuple<TypesA...>& i_lhs,const mpl::sequence<IndexsB...>&,tuple<TypesB...>& i_rhs)
{
    return tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...>(i_srcSeq,i_lhs.template get<IndexsA>() ...,i_rhs.template get<IndexsB>() ...);
}
template<typename ... FinalTypes,size_t ... FromIndexs,size_t ... ToIndexs,size_t ... Indexs,typename ... Types,typename ... Args>
constexpr tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge_args(const mpl::sequence<FromIndexs...>& i_srcSeq,const mpl::sequence<ToIndexs...>&,const mpl::sequence<Indexs...>&,tuple<Types...>& i_lhs,Args&& ... i_args)
{
    return tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...>(i_srcSeq,i_lhs.template get<Indexs>() ...,std::forward<Args>(i_args) ...);
}
template<typename ... TypesA,typename ... TypesB>
constexpr tuple<TypesA...,TypesB...> merge(const tuple<TypesA...>& i_lhs,const tuple<TypesB...>& i_rhs)
{
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesA...>()>::type sequenceA;
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesB...>()>::type sequenceB;
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesA...>() + mpl::get_num_types<TypesB...>()>::type total_sequence;

    return merge<TypesA...,TypesB...>(total_sequence{},total_sequence{},sequenceA{},i_lhs,sequenceB{},i_rhs);
}
template<typename ... FinalTypes,size_t ... FromIndexs,size_t ... ToIndexs,typename ... TypesA,typename ... TypesB>
constexpr tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge(const mpl::sequence<FromIndexs...>& i_srcSeq,const mpl::sequence<ToIndexs...>& i_destSeq,const tuple<TypesA...>& i_lhs,const tuple<TypesB...>& i_rhs)
{
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesA...>()>::type sequenceA;
    typedef typename mpl::make_sequence<0,mpl::get_num_types<TypesB...>()>::type sequenceB;

    return merge<FinalTypes...>(i_srcSeq,i_destSeq,sequenceA{},const_cast<tuple<TypesA...>&>(i_lhs),sequenceB{},const_cast<tuple<TypesB...>&>(i_rhs));
}
template<typename ... Types,typename ... Args>
constexpr tuple<Types...,Args...> merge_args(const tuple<Types...>& i_lhs,Args&& ... i_args)
{
    typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>() + mpl::get_num_types<Args...>()>::type total_sequence;
    typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>()>::type sequence_t;

    return merge_args<Types...,Args...>(total_sequence{},total_sequence{},sequence_t{},const_cast<tuple<Types...>&>(i_lhs),std::forward<Args>(i_args) ...);
}
template<typename ... FinalTypes,size_t ... FromIndexs,size_t ... ToIndexs,typename ... Types,typename ... Args>
constexpr tuple<typename mpl::nth_type_of<ToIndexs,FinalTypes...>::type ...> merge_args(const mpl::sequence<FromIndexs...>& i_srcSeq,const mpl::sequence<ToIndexs...>& i_destSeq,const tuple<Types...>& i_lhs,Args&& ... i_args)
{
    typedef typename mpl::make_sequence<0,mpl::get_num_types<Types...>()>::type sequence_t;

    return merge_args<FinalTypes...>(i_srcSeq,i_destSeq,sequence_t{},const_cast<tuple<Types...>&>(i_lhs),std::forward<Args>(i_args) ...);
}

}
