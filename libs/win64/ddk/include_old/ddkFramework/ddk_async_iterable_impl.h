#pragma once

#include "ddk_iterable_impl_interface.h"
#include "ddk_sync_executor_impl.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
class async_iterable_impl : public iterable_impl_interface<typename Traits::iterable_base_traits>
{
    typedef typename Traits::iterable_base_traits iterable_base_traits;

public:
    typedef typename Traits::value_type value_type;
    typedef typename Traits::reference reference;
    typedef typename Traits::const_reference const_reference;
    typedef typename Traits::action action;

    async_iterable_impl(iterable_impl_dist_ref<iterable_base_traits> i_iterableRef, const function<reference(private_reference)>& i_transform);

private:
    void iterate_impl(const function<action(reference)>& i_try) override;
    void iterate_impl(const function<action(const_reference)>& i_try) const override;

    iterable_impl_dist_ref<private_iterable_base_traits> m_iterableRef;

};

}
}

#include "ddk_async_iterable_impl.inl"
