#pragma once

#include "ddk_function.h"
#include "ddk_connection_base.h"
#include <stdio.h>
#include <tuple>
#include <utility>
#include "ddk_template_helper.h"
#include "ddk_signal_connector.h"

namespace ddk
{

template<typename>
class signal;

template<typename>
class signal_functor;

template<typename ... Types>
class signal_functor<void(Types...)> : public detail::connection_base
{
	typedef std::tuple<typename std::remove_const<typename std::remove_reference<Types>::type>::type ...> tuple_t;

public:
    signal_functor(const ddk::function<void(Types...)>& call, const detail::signal_connector& i_connector);
    signal_functor(const signal_functor&) = delete;
    signal_functor(signal_functor&&) = delete;
	template<typename ... Args>
    void execute(Args&& ... i_args) const;
    void execute_tuple(const tuple_t& i_args) const;
    signal_functor& operator=(const signal_functor&) = delete;
    signal_functor& operator=(signal_functor&&) = delete;
    bool operator==(const signal_functor& other) const;

private:
    template<size_t ... Seq>
    inline void _execute(const tuple_t& i_args, const mpl::sequence<Seq...>&) const;

    ddk::function<void(Types...)>  m_call;
};

}

#include "ddk_signal_functor.inl"