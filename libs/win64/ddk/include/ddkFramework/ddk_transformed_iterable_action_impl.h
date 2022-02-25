#pragma once

namespace ddk
{
namespace detail
{

template<typename PublicTraits, typename PrivateTraits>
class transformed_iterable_action_impl : public iterable_impl_interface<typename PublicTraits::iterable_base_traits>
{
    static_assert(std::is_same<typename PublicTraits::reference,typename PrivateTraits::reference>::value, "Values from both public and private traits shall be the same");

    typedef typename PrivateTraits::iterable_base_traits private_iterable_base_traits;
    typedef typename PrivateTraits::action private_action;

public:
    typedef typename PublicTraits::value_type value_type;
    typedef typename PublicTraits::reference reference;
    typedef typename PublicTraits::const_reference const_reference;
    typedef typename PublicTraits::action action;

    transformed_iterable_action_impl(iterable_impl_dist_ref<private_iterable_base_traits> i_iterableRef, const function<private_action(action)>& i_transform);

private:
    void iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) override;
    void iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const override;
    size_t size() const override;
    bool empty() const override;

    iterable_impl_dist_ref<private_iterable_base_traits> m_iterableRef;
    const function<private_action(action)> m_transform;
};

}
}

#include "ddk_transformed_iterable_action_impl.inl"
