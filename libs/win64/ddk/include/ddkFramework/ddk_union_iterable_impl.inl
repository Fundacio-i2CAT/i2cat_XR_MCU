
#include "ddk_async_exceptions.h"
#include "ddk_fiber_utils.h"

namespace ddk
{
namespace detail
{

template<size_t Index, typename ... Iterables>
void navigate(tuple<Iterables...>& i_iterables, const function<typename union_iterable_impl<Iterables...>::action(typename union_iterable_impl<Iterables...>::reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
    typedef typename mpl::nth_type_of<Index,Iterables...>::type curr_iterable_type;
    typedef typename curr_iterable_type::reference curr_reference;
    typedef typename union_iterable_impl<Iterables...>::reference reference;

    try
    {
        if constexpr(std::is_same<reference,curr_reference>::value)
        {
            lend(i_iterables.template get<Index>())->iterate_impl(i_try,i_initialAction,i_actionStatePtr);
        }
        else
        {
            lend(i_iterables.template get<Index>())->iterate_impl(make_composition(i_try,make_function([](curr_reference i_value) -> reference { return i_value; })),i_initialAction,i_actionStatePtr);
        }
    }
    catch(const suspend_exception&)
    {
    }
}
template<size_t Index, typename ... Iterables>
void navigate(const tuple<Iterables...>& i_iterables, const function<typename union_iterable_impl<Iterables...>::action(typename union_iterable_impl<Iterables...>::const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
    typedef typename mpl::nth_type_of<Index,Iterables...>::type curr_iterable_type;
    typedef typename curr_iterable_type::const_reference curr_const_reference;
    typedef typename union_iterable_impl<Iterables...>::const_reference const_reference;

    try
    {
        if constexpr (std::is_same<const_reference,curr_const_reference>::value)
        {
            lend(i_iterables.template get<Index>())->iterate_impl(i_try,i_initialAction,i_actionStatePtr);
        }
        else
        {
            lend(i_iterables.template get<Index>())->iterate_impl(make_composition(i_try,make_function([](curr_const_reference i_value) -> const_reference { return i_value; })),i_initialAction,i_actionStatePtr);
        }
    }
    catch(const suspend_exception&)
    {
    }
}

template<size_t ... Indexs, typename ... Iterables>
const typename union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::size_t_func union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::s_size_funcs[s_num_iterables] = { &union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::template iterable_size<Indexs> ...};

template<size_t ... Indexs, typename ... Iterables>
union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::union_iterable_visitor_type(size_t i_currIterableIndex, const tuple<Iterables...>& i_iterables)
: m_currIterableIndex(i_currIterableIndex)
, m_iterables(i_iterables)
{
}
template<size_t ... Indexs,typename ... Iterables>
template<typename T>
std::pair<size_t,shift_action> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::operator()(const T& i_action) const
{
    suspend();

    return std::make_pair(iterable_state::npos,go_no_place);
}
template<size_t ... Indexs,typename ... Iterables>
std::pair<size_t,shift_action> union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::operator()(const ShiftActionError& i_action) const
{
    const auto pendingShift = i_action.get_pending_shift();

    const size_t newIterableIndex = (pendingShift > 0) ? m_currIterableIndex + 1 : m_currIterableIndex - 1;

    if(newIterableIndex < s_num_iterables)
    {
        return std::make_pair(newIterableIndex,go_to_place((pendingShift > 0) ? pendingShift - 1 : pendingShift + 1));
    }
    else
    {
        return std::make_pair(iterable_state::npos,go_no_place);
    }
}
template<size_t ... Indexs, typename ... Iterables>
template<size_t Index>
size_t union_iterable_visitor_type<mpl::sequence<Indexs...>,Iterables...>::iterable_size(const tuple<Iterables...>& i_iterables)
{
    return i_iterables.template get<Index>().size();
}

template<typename ... Iterables>
union_iterable_impl<Iterables...>::union_iterable_impl(const Iterables& ... i_iterables)
: m_iterables(i_iterables...)
{
}
template<typename ... Iterables>
union_iterable_impl<Iterables...>::union_iterable_impl(const tuple<Iterables...>& i_tupleIterable)
: m_iterables(i_tupleIterable)
{
}
template<typename ... Iterables>
const tuple<Iterables...>& union_iterable_impl<Iterables...>::get_iterables() const
{
    return m_iterables;
}
template<typename ... Iterables>
tuple<Iterables...>& union_iterable_impl<Iterables...>::get_iterables()
{
    return m_iterables;
}
template<typename ... Iterables>
void union_iterable_impl<Iterables...>::iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initialAction,i_actionStatePtr);
}
template<typename ... Iterables>
void union_iterable_impl<Iterables...>::iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initialAction,i_actionStatePtr);
}
template<typename ... Iterables>
template<size_t ... Indexs>
void union_iterable_impl<Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
    typedef void(*navigate_func)(tuple<Iterables...>&, const function<action(reference)>&,const shift_action&,action_state_lent_ptr);

    static const navigate_func s_navFuncs[s_num_iterables] = { navigate<Indexs,Iterables...> ...};

    size_t iterableIndex= (i_initialAction.shifting() == -1) ? s_num_iterables - 1 : 0;
	shift_action initialAction = i_initialAction;

    while(iterableIndex != iterable_state::npos)
    {
        (*s_navFuncs[iterableIndex])(m_iterables,i_try,initialAction,i_actionStatePtr);

        union_iterable_visitor<Iterables...> unionVisitor(iterableIndex,m_iterables);

        const std::pair<size_t,shift_action> currIndexes = i_actionStatePtr->visit(unionVisitor);

        iterableIndex = currIndexes.first;
		initialAction = currIndexes.second;
    }

    suspend();
}
template<typename ... Iterables>
template<size_t ... Indexs>
void union_iterable_impl<Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const
{
	typedef void(*navigate_func)(const tuple<Iterables...>&, const function<action(const_reference)>&, const shift_action&,action_state_lent_ptr);

	static const navigate_func s_navFuncs[s_num_iterables] = { &navigate<Indexs,Iterables...> ... };

    size_t iterableIndex= 0;
	shift_action initialAction = i_initialAction;

    while(iterableIndex != iterable_state::npos)
    {
        (*s_navFuncs[iterableIndex])(m_iterables,i_try,initialAction,i_actionStatePtr);

        union_iterable_visitor<Iterables...> unionVisitor(iterableIndex,m_iterables);

        const std::pair<size_t,shift_action> currIndexes = i_actionStatePtr->visit(unionVisitor);

        iterableIndex = currIndexes.first;
		initialAction = currIndexes.second;
    }

    suspend();
}
template<typename ... Iterables>
size_t union_iterable_impl<Iterables...>::size() const
{
	TODO("Pending");

    return 0;
}
template<typename ... Iterables>
bool union_iterable_impl<Iterables...>::empty() const
{
	TODO("Pending");

	return true;
}

}
}
