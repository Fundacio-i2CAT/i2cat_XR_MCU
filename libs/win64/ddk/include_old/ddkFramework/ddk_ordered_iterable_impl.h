#pragma once

namespace ddk
{
namespace detail
{

template<typename T>
class iterable_order
{
public:
	iterable_order(const T& i_order);

	const T& get_order() const;

private:
	const T m_order;
};

template<typename T, typename Traits>
class ordered_iterable_impl : public iterable_impl_interface<typename Traits::iterable_base_traits>
{
	typedef typename Traits::iterable_base_traits iterable_base_traits;

public:
	typedef typename Traits::value_type value_type;
	typedef typename Traits::reference reference;
	typedef typename Traits::const_reference const_reference;
	typedef typename Traits::action action;

	ordered_iterable_impl(iterable_impl_dist_ref<iterable_base_traits> i_iterableRef, const T& i_actionResolver);

private:
	void iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) override;
	void iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const override;
	size_t size() const override;
	bool empty() const override;

	iterable_impl_dist_ref<iterable_base_traits> m_iterableRef;
	const T m_actionResolver;
};

}
}

#include "ddk_ordered_iterable_impl.inl"
