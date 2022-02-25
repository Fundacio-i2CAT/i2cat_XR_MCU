
namespace ddk
{
namespace detail
{

template<size_t Index, typename ... Iterables>
typename mpl::nth_type_of<Index, Iterables...>::type::reference intersection_navigate(tuple<Iterables...>* i_iterables, const shift_action& i_initialAction)
{
	typedef typename mpl::nth_type_of<Index, Iterables...>::type::reference curr_reference;
	typedef typename mpl::nth_type_of<Index, Iterables...>::type::reference curr_reference;

	i_iterables->template get<Index>().iterate(make_function([](curr_reference i_value){ yield<curr_reference>(i_value); }),i_initialAction);

	suspend();

	return crash_on_return<curr_reference>::value();
}
template<size_t Index, typename ... Iterables>
typename mpl::nth_type_of<Index, Iterables...>::type::const_reference const_intersection_navigate(const tuple<Iterables...>* i_iterables, const shift_action& i_initialAction)
{
	typedef typename mpl::nth_type_of<Index, Iterables...>::type::const_reference curr_const_reference;
	typedef typename mpl::nth_type_of<Index, Iterables...>::type::const_reference curr_const_reference;

	i_iterables->template get<Index>().iterate(make_function([](curr_const_reference i_value) { yield<curr_const_reference>(i_value); }),i_initialAction);

	suspend();

	return crash_on_return<curr_const_reference>::value();
}

template<typename ActionAdapter,typename ... Iterables>
intersection_iterable_impl<ActionAdapter,Iterables...>::intersection_iterable_impl(const ActionAdapter& i_adapter, const Iterables& ... i_iterables)
: m_iterables(i_iterables...)
, m_actionAdapter(i_adapter)
{
}
template<typename ActionAdapter, typename ... Iterables>
intersection_iterable_impl<ActionAdapter,Iterables...>::intersection_iterable_impl(const ActionAdapter& i_adapter, const tuple<Iterables...>& i_tupleIterable)
: m_iterables(i_tupleIterable)
, m_actionAdapter(i_adapter)
{
}
template<typename ActionAdapter,typename ... Iterables>
intersection_iterable_impl<ActionAdapter,Iterables...>::intersection_iterable_impl(ActionAdapter&& i_adapter,const Iterables& ... i_iterables)
: m_iterables(i_iterables...)
, m_actionAdapter(std::move(i_adapter))
{
}
template<typename ActionAdapter,typename ... Iterables>
intersection_iterable_impl<ActionAdapter,Iterables...>::intersection_iterable_impl(ActionAdapter&& i_adapter,const tuple<Iterables...>& i_tupleIterable)
: m_iterables(i_tupleIterable)
, m_actionAdapter(std::move(i_adapter))
{
}
template<typename ActionAdapter, typename ... Iterables>
const tuple<Iterables...>& intersection_iterable_impl<ActionAdapter,Iterables...>::get_iterables() const
{
    return m_iterables;
}
template<typename ActionAdapter, typename ... Iterables>
tuple<Iterables...>& intersection_iterable_impl<ActionAdapter,Iterables...>::get_iterables()
{
    return m_iterables;
}
template<typename ActionAdapter, typename ... Iterables>
void intersection_iterable_impl<ActionAdapter,Iterables...>::iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initialAction,i_actionStatePtr);
}
template<typename ActionAdapter, typename ... Iterables>
void intersection_iterable_impl<ActionAdapter,Iterables...>::iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const
{
    iterate_impl(typename mpl::make_sequence<0,s_num_iterables>::type{},i_try,i_initialAction,i_actionStatePtr);
}
template<typename ActionAdapter, typename ... Iterables>
size_t intersection_iterable_impl<ActionAdapter,Iterables...>::size() const
{
	TODO("Pending");

	return 0;
}
template<typename ActionAdapter, typename ... Iterables>
bool intersection_iterable_impl<ActionAdapter,Iterables...>::empty() const
{
	TODO("Pending");

	return true;
}
template<typename ActionAdapter, typename ... Iterables>
template<size_t ... Indexs>
void intersection_iterable_impl<ActionAdapter,Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
    tuple<awaitable<typename Iterables::reference>...> awaitableTuple(await(make_function(intersection_navigate<Indexs,Iterables...>,&m_iterables,i_initialAction))...);
    tuple<awaited_result<typename Iterables::reference>...> awaitableResultTuple;
    action currAction = traits::default_action();

    do
    {
        if((awaitableResultTuple.template set<Indexs>(resume(awaitableTuple.template get<Indexs>())) && ...))
        {
            auto currActionComposition = m_actionAdapter.template resolve<Indexs...>(eval(i_try,make_values_tuple(awaitableResultTuple.template get<Indexs>().get() ...)));

			//update current action
			(m_iterables.template get<Indexs>().forward_action(currActionComposition[Indexs]) && ...);
        }
        else
        {
            break;
        }
    }
    while(true);
}
template<typename ActionAdapter, typename ... Iterables>
template<size_t ... Indexs>
void intersection_iterable_impl<ActionAdapter,Iterables...>::iterate_impl(const mpl::sequence<Indexs...>&, const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const
{
    tuple<awaitable<typename Iterables::const_reference>...> awaitableTuple(await(make_function(const_intersection_navigate<Indexs,Iterables...>,&m_iterables,i_initialAction))...);
    tuple<awaited_result<typename Iterables::const_reference>...> awaitableResultTuple;
    action currAction = traits::default_action();

    do
    {
        (m_iterables.template get<Indexs>().forward_action(currAction) && ...);

        if((awaitableResultTuple.template set<Indexs>(resume(awaitableTuple.template get<Indexs>())) && ...))
        {
            auto currActionComposition = m_actionAdapter.template resolve<Indexs...>(eval(i_try,make_values_tuple(awaitableResultTuple.template get<Indexs>().get() ...)));

            //update current action
            (m_iterables.template get<Indexs>().forward_action(currActionComposition[Indexs]) && ...);
        }
        else
        {
            break;
        }
    }
    while(true);
}

}
}
