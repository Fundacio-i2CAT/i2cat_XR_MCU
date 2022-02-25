#pragma once

#include "ddk_sync_executor.h"
#include "ddk_void.h"
#include "ddk_function_template_helper.h"

namespace ddk
{

template<typename Return>
using async_dist_ref = typename async_executor<Return>::async_dist_ref;
template<typename Return>
using async_const_dist_ref = typename async_executor<Return>::async_const_dist_ref;

template<typename Return>
inline async_dist_ref<Return> async(const ddk::function<Return()>& i_function);

//void version
async_dist_ref<void> async(const ddk::function<void()>& i_function);

}

#include "ddk_async.inl"
