#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_function.h"
#include "ddk_iterable_action.h"
#include "ddk_iterable_valued_traits.h"
#include "ddk_iterable_state.h"
#include "ddk_await_executor.h"
#include "ddk_arena.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class iterable : protected iterable_interface
{
    template<typename TTraits>
    friend class iterable;
	friend typename Traits::iterable_tag iterable_tag_resolver(const iterable&);
	friend inline iterable_impl_lent_ref<typename Traits::iterable_base_traits> lend(iterable<Traits>& i_iterable)
    {
        return lend(i_iterable.m_iterableImpl);
    }
    friend inline const iterable_impl_lent_ref<typename Traits::iterable_base_traits> lend(const iterable<Traits>& i_iterable)
    {
        return lend(i_iterable.m_iterableImpl);
    }
    friend inline iterable_impl_dist_ref<typename Traits::iterable_base_traits> share(iterable<Traits>& i_iterable)
    {
        return i_iterable.m_iterableImpl;
    }
    friend inline const iterable_impl_dist_ref<typename Traits::iterable_base_traits> share(const iterable<Traits>& i_iterable)
    {
        return i_iterable.m_iterableImpl;
    }

public:
    typedef typename Traits::iterable_value iterable_value;
    typedef typename Traits::iterable_const_value iterable_const_value;
    typedef typename Traits::iterable_base_traits iterable_base_traits;
    typedef typename Traits::value_type value_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::action action;
    typedef Traits traits;

    iterable(iterable_impl_dist_ref<iterable_base_traits> i_iterableImpl);
    iterable(const iterable&);
    template<typename TTraits>
    iterable(const iterable<TTraits>& other);
	~iterable();

    TEMPLATE(typename Function)
    REQUIRES(IS_CALLABLE(Function))        
    action_result iterate(Function&& i_try, const shift_action& i_initialAction = go_to_place);
    TEMPLATE(typename Function)
    REQUIRES(IS_CALLABLE(Function))
    action_result iterate(Function&& i_try, const shift_action& i_initialAction = go_to_place) const;
    TEMPLATE(typename Function)
    REQUIRES(IS_CALLABLE(Function))
    action_result co_iterate(Function&& i_try,const shift_action& i_initialAction = go_to_place);
    TEMPLATE(typename Function)
    REQUIRES(IS_CALLABLE(Function))
    action_result co_iterate(Function&& i_try,const shift_action& i_initialAction = go_to_place) const;
    bool inline forward_action(action i_action) const;
    bool inline operator==(const std::nullptr_t&) const;
    bool inline operator!=(const std::nullptr_t&) const;
    size_t inline size() const;
    bool inline empty() const;

private:
    iterable_state& get_state() override;
    const iterable_state& get_state() const override;
    action_state_lent_ref get_action_state() override;
    action_state_const_lent_ref get_action_state() const override;

    action private_iterate(reference i_value);
    action private_iterate(const_reference i_value) const;
    reference resolve_action(const action&);
    const_reference resolve_action(const action&) const;

	mutable lendable<action_state> m_actionState;
    iterable_impl_dist_ref<iterable_base_traits> m_iterableImpl;
    mutable iterable_state m_iterableState;
	mutable detail::await_executor<void> m_executor;
	mutable typed_arena<reference> m_iterableValueContainer;
    mutable action m_currAction = iterable_base_traits::default_action();
};

}

template<typename T>
using const_forward_iterable = detail::iterable<detail::const_forward_iterable_valued_traits<T>>;

template<typename T>
using forward_iterable = detail::iterable<detail::forward_iterable_valued_traits<T>>;

template<typename T>
using const_bidirectional_iterable = detail::iterable<detail::const_bidirectional_iterable_valued_traits<T>>;

template<typename T>
using bidirectional_iterable = detail::iterable<detail::bidirectional_iterable_valued_traits<T>>;

template<typename T>
using const_random_access_iterable = detail::iterable<detail::const_random_access_iterable_valued_traits<T>>;

template<typename T>
using random_access_iterable = detail::iterable<detail::random_access_iterable_valued_traits<T>>;

}

#include "ddk_iterable.inl"
#include "ddk_iterable_resolver.h"
#include "ddk_iterable_utils.h"
#include "ddk_view_utils.h"
