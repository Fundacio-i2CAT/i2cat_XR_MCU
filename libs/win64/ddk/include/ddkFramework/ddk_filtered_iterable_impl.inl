
#include "ddk_iterable_exceptions.h"
#include "ddk_iterable_interface_utils.h"

namespace ddk
{
namespace detail
{

template<typename Function>
iterable_filter<Function>::iterable_filter(const Function& i_filter)
: m_filter(i_filter)
{
}
template<typename Function>
Function iterable_filter<Function>::get_filter() const
{
	return m_filter;
}

template<typename Traits, typename Function>
filtered_iterable_impl<Traits,Function>::filtered_iterable_impl(iterable_impl_dist_ref<iterable_base_traits> i_iterableRef, const Function& i_filter)
: m_iterableRef(i_iterableRef)
, m_filter(i_filter)
{
}
template<typename Traits, typename Function>
void filtered_iterable_impl<Traits,Function>::iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
    typedef typename mpl::make_sequence<0,mpl::aqcuire_callable_args_type<Function>::type::size()>::type range_seq;
    
    m_iterableRef->iterate_impl(make_function([i_try,this,actionResult=action(i_initialAction)](reference i_value) mutable -> action { if(call_iterable_payload(range_seq{},m_filter,i_value)) actionResult = eval(i_try,i_value); if(actionResult.template is_base_of<shift_action>()) actionResult.template get_as<shift_action>().set_step_by_step(true); return actionResult; }),i_initialAction,i_actionStatePtr);
}
template<typename Traits, typename Function>
void filtered_iterable_impl<Traits,Function>::iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const
{
    typedef typename mpl::make_sequence<0,mpl::aqcuire_callable_args_type<Function>::type::size()>::type range_seq;
    
    m_iterableRef->iterate_impl(make_function([i_try,this,actionResult=action(i_initialAction)](const_reference i_value) mutable -> action { if(call_iterable_payload(range_seq{},m_filter,i_value)) actionResult = eval(i_try, i_value); if(actionResult.template is_base_of<shift_action>()) actionResult.template get_as<shift_action>().set_step_by_step(true); return actionResult; }),i_initialAction,i_actionStatePtr);
}
template<typename Traits, typename Function>
size_t filtered_iterable_impl<Traits,Function>::size() const
{
    typedef typename mpl::make_sequence<0,mpl::aqcuire_callable_args_type<Function>::type::size()>::type range_seq;

    size_t res = 0;

    const auto sizeChecker = [&](const_reference i_value) mutable -> action
                        {
                            if(call_iterable_payload(range_seq{},m_filter,i_value))
                            {
                                ++res;
                            }

                            return go_next_place;
                        };

	lendable<action_state> actionState;
    m_iterableRef->iterate_impl(sizeChecker,go_no_place,lend(actionState));

    return res;
}
template<typename Traits, typename Function>
bool filtered_iterable_impl<Traits,Function>::empty() const
{
    typedef typename mpl::make_sequence<0,mpl::aqcuire_callable_args_type<Function>::type::size()>::type range_seq;

    bool res = true;

    const auto emptyChecker = [&](const_reference i_value) mutable -> action
                        {
                            if(call_iterable_payload(range_seq{},m_filter,i_value))
                            {
                                res = false;

                                stop_iteration();
                            }

							return go_next_place;
                        };

	lendable<action_state> actionState;
	m_iterableRef->iterate_impl(emptyChecker,go_no_place,lend(actionState));

    return res;
}

}
}
