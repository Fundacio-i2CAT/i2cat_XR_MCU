#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_function.h"

namespace ddk
{
namespace detail
{

template<typename Function>
class iterable_filter
{
	static_assert(std::is_same<typename Function::return_type,bool>::value, "You shall provide a boolen function");

public:
	iterable_filter(const Function& i_filter);

	Function get_filter() const;

private:
	const Function m_filter;
};


template<typename Traits, typename Function>
class filtered_iterable_impl : public iterable_impl_interface<typename Traits::iterable_base_traits>
{
    typedef typename Traits::iterable_base_traits iterable_base_traits;

public:
    typedef typename Traits::value_type value_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::action action;

    filtered_iterable_impl(iterable_impl_dist_ref<iterable_base_traits> i_iterableRef, const Function& i_filter);

private:
    void iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) override;
    void iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const override;
    size_t size() const override;
    bool empty() const override;

    iterable_impl_dist_ref<iterable_base_traits> m_iterableRef;
    const Function m_filter;
};

}
}

#include "ddk_filtered_iterable_impl.inl"
