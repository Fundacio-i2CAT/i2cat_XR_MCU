#pragma once

#include "ddk_iterable_traits.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_function.h"
#include "ddk_iterable_state.h"
#include "ddk_iterable_action_state.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class iterable_impl_interface
{
public:
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::action action;
    typedef Traits traits;

    virtual ~iterable_impl_interface() = default;

    virtual void iterate_impl(const function<action(reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr = nullptr) = 0;
    virtual void iterate_impl(const function<action(const_reference)>& i_try, const shift_action& i_initialAction, action_state_lent_ptr i_actionStatePtr = nullptr) const = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
};

template<typename Traits>
using iterable_impl_dist_ref = distributed_reference_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_dist_ref = distributed_reference_wrapper<const iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_dist_ptr = distributed_pointer_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_dist_ptr = distributed_pointer_wrapper<const iterable_impl_interface<Traits>>;

template<typename Traits>
using iterable_impl_lent_ref = lent_reference_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_lent_ref = lent_reference_wrapper<const iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_lent_ptr = lent_pointer_wrapper<iterable_impl_interface<Traits>>;
template<typename Traits>
using iterable_impl_const_lent_ptr = lent_pointer_wrapper<const iterable_impl_interface<Traits>>;

template<typename T>
using forward_iterable_impl_interface = iterable_impl_interface<detail::forward_iterable_traits<T>>;

template<typename T>
using forward_iterable_impl_dist_ref = distributed_reference_wrapper<iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_const_dist_ref = distributed_reference_wrapper<const iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_dist_ptr = distributed_pointer_wrapper<iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_const_dist_ptr = distributed_pointer_wrapper<const iterable_impl_interface<detail::forward_iterable_traits<T>>>;

template<typename T>
using forward_iterable_impl_lent_ref = lent_reference_wrapper<iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_const_lent_ref = lent_reference_wrapper<const iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_lent_ptr = lent_pointer_wrapper<iterable_impl_interface<detail::forward_iterable_traits<T>>>;
template<typename T>
using forward_iterable_impl_const_lent_ptr = lent_pointer_wrapper<const iterable_impl_interface<detail::forward_iterable_traits<T>>>;

template<typename IterableImpl, typename ... Args>
inline distributed_reference_wrapper<IterableImpl> make_iterable_impl(Args&& ... i_args);

}
}

#include "ddk_iterable_impl_interface.inl"
