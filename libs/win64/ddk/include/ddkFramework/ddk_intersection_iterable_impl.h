#pragma once

#include "ddk_tuple.h"
#include "ddk_iterable_impl_interface.h"
#include "ddk_iterable_valued_traits.h"

namespace ddk
{
namespace detail
{

template<typename ActionAdapter, typename ... Iterables>
class intersection_iterable_impl : public iterable_impl_interface<intersection_iterable_base_traits<typename Iterables::traits ...>>
{
    static const size_t s_num_iterables = tuple<Iterables...>::size();
    typedef iterable_impl_interface<intersection_iterable_base_traits<typename Iterables::traits ...>> base_t;
    using typename base_t::traits;
    typedef typename intersection_iterable_traits<typename Iterables::traits ...>::iterable_value iterable_value;
    typedef typename intersection_iterable_traits<typename Iterables::traits ...>::iterable_const_value iterable_const_value;

public:
    using typename base_t::reference;
    using typename base_t::const_reference;
    using typename base_t::action;

    intersection_iterable_impl(const ActionAdapter& i_adapter, const Iterables& ... i_iterables);
    intersection_iterable_impl(const ActionAdapter& i_adapter, const tuple<Iterables...>& i_tupleIterable);
    intersection_iterable_impl(ActionAdapter&& i_adapter,const Iterables& ... i_iterables);
    intersection_iterable_impl(ActionAdapter&& i_adapter,const tuple<Iterables...>& i_tupleIterable);
    intersection_iterable_impl(const intersection_iterable_impl&) = default;
	intersection_iterable_impl(intersection_iterable_impl&&) = default;

    const tuple<Iterables...>& get_iterables() const;
    tuple<Iterables...>& get_iterables();

private:
    void iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) override;
    void iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const override;
    size_t size() const override;
    bool empty() const override;

    template<size_t ... Indexs>
    inline void iterate_impl(const mpl::sequence<Indexs...>&, const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr);
    template<size_t ... Indexs>
    inline void iterate_impl(const mpl::sequence<Indexs...>&, const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr) const;

    tuple<Iterables...> m_iterables;
    ActionAdapter m_actionAdapter;
};
template<typename ActionAdapter,typename ... Iterables>
intersection_iterable_impl(const ActionAdapter&, const Iterables& ...) ->intersection_iterable_impl<ActionAdapter,Iterables...>;
template<typename ActionAdapter,typename ... Iterables>
intersection_iterable_impl(ActionAdapter&&,const Iterables& ...)->intersection_iterable_impl<ActionAdapter,Iterables...>;

}
}

#include "ddk_intersection_iterable_impl.inl"
