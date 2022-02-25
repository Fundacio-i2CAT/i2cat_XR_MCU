#pragma once

#include "ddk_awaitable.h"
#include "ddk_template_helper.h"

namespace ddk
{

template<typename Return, typename Result = awaited_result<Return>>
awaitable<Return,Result> await(const ddk::function<Return()>& i_function);

template<typename Return, typename Result = awaited_result<Return>>
awaitable<Return,Result> await(const ddk::function<Return()>& i_function, const detail::this_fiber_t& i_fiber);

}

#include "ddk_await.inl"
