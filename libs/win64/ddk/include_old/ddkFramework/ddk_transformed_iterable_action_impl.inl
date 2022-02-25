
namespace ddk
{
namespace detail
{

template<typename PublicTraits, typename PrivateTraits>
transformed_iterable_action_impl<PublicTraits,PrivateTraits>::transformed_iterable_action_impl(iterable_impl_dist_ref<private_iterable_base_traits> i_iterableRef, const function<private_action(action)>& i_transform)
: m_iterableRef(i_iterableRef)
, m_transform(i_transform)
{
}
template<typename PublicTraits, typename PrivateTraits>
void transformed_iterable_action_impl<PublicTraits,PrivateTraits>::iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
    m_iterableRef->iterate_impl(make_composition(m_transform,i_try),i_initialAction,i_actionStatePtr);
}
template<typename PublicTraits, typename PrivateTraits>
void transformed_iterable_action_impl<PublicTraits,PrivateTraits>::iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const
{
    m_iterableRef->iterate_impl(make_composition(m_transform,i_try),i_initialAction,i_actionStatePtr);
}
template<typename PublicTraits, typename PrivateTraits>
size_t transformed_iterable_action_impl<PublicTraits,PrivateTraits>::size() const
{
    return m_iterableRef->size();
}
template<typename PublicTraits, typename PrivateTraits>
bool transformed_iterable_action_impl<PublicTraits,PrivateTraits>::empty() const
{
    return m_iterableRef->empty();
}

}
}
