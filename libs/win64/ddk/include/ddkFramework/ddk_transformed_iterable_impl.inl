
#include "ddk_iterable_interface_utils.h"

namespace ddk
{
namespace detail
{

template<typename Function>
iterable_transform<Function>::iterable_transform(const Function& i_transform)
: m_transform(i_transform)
{
}
template<typename Function>
Function iterable_transform<Function>::get_transform() const
{
	return m_transform;
}

template<typename PublicTraits, typename PrivateTraits, typename Function>
transformed_iterable_impl<PublicTraits,PrivateTraits,Function>::transformed_iterable_impl(iterable_impl_dist_ref<private_iterable_base_traits> i_iterableRef, const Function& i_transform)
: m_iterableRef(i_iterableRef)
, m_transform(i_transform)
{
}
template<typename PublicTraits, typename PrivateTraits, typename Function>
void transformed_iterable_impl<PublicTraits,PrivateTraits,Function>::iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
	typedef typename mpl::make_sequence<0,mpl::aqcuire_callable_args_type<Function>::type::size()>::type range_seq;

	m_iterableRef->iterate_impl(make_composition(i_try,make_function([this](private_reference i_value) { return call_iterable_payload(range_seq{},m_transform,i_value); })),i_initialAction,i_actionStatePtr);
}
template<typename PublicTraits, typename PrivateTraits, typename Function>
void transformed_iterable_impl<PublicTraits,PrivateTraits,Function>::iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const
{
	typedef typename mpl::make_sequence<0,mpl::aqcuire_callable_args_type<Function>::type::size()>::type range_seq;

	m_iterableRef->iterate_impl(make_composition(i_try,make_function([this](private_const_reference i_value) { return call_iterable_payload(range_seq{},m_transform,i_value); })),i_initialAction,i_actionStatePtr);
}
template<typename PublicTraits, typename PrivateTraits, typename Function>
size_t transformed_iterable_impl<PublicTraits,PrivateTraits,Function>::size() const
{
    return m_iterableRef->size();
}
template<typename PublicTraits, typename PrivateTraits, typename Function>
bool transformed_iterable_impl<PublicTraits,PrivateTraits,Function>::empty() const
{
    return m_iterableRef->empty();
}

}
}
