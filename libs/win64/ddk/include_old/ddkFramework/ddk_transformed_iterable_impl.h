#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_function.h"

namespace ddk
{
namespace detail
{

template<typename Function>
class iterable_transform
{
public:
	iterable_transform(const Function& i_transform);

	Function get_transform() const;

private:
	const Function m_transform;
};

template<typename PublicTraits, typename PrivateTraits, typename Function>
class transformed_iterable_impl : public iterable_impl_interface<typename PublicTraits::iterable_base_traits>
{
    static_assert(std::is_same<typename PublicTraits::action,typename PrivateTraits::action>::value, "Actions from both public and private traits shall be the same");
    static_assert(std::is_constructible<typename mpl::aqcuire_callable_return_type<Function>::type,typename PublicTraits::reference>::value || std::is_convertible<typename mpl::aqcuire_callable_return_type<Function>::type,typename PublicTraits::reference>::value, "You shall provider consistent transformations");

    typedef typename PrivateTraits::iterable_base_traits private_iterable_base_traits;
    typedef typename PrivateTraits::reference private_reference;
    typedef typename PrivateTraits::const_reference private_const_reference;

public:
    typedef typename PublicTraits::value_type value_type;
    typedef typename PublicTraits::reference reference;
    typedef typename PublicTraits::const_reference const_reference;
    typedef typename PublicTraits::action action;

    transformed_iterable_impl(iterable_impl_dist_ref<private_iterable_base_traits> i_iterableRef, const Function& i_transform);

private:
    void iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) override;
    void iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const override;
    size_t size() const override;
    bool empty() const override;

    iterable_impl_dist_ref<private_iterable_base_traits> m_iterableRef;
    const Function m_transform;
};

}
}

#include "ddk_transformed_iterable_impl.inl"
