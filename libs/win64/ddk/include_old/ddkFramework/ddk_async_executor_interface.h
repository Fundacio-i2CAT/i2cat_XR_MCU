#pragma once

#include "ddk_sync_executor_context_interface.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_thread.h"
#include "ddk_fiber.h"
#include "ddk_embedded_type.h"
#include "ddk_result.h"

namespace ddk
{

class async_interface_base
{
public:
	virtual ~async_interface_base() = default;
	virtual bool notify() = 0;
	virtual executor_context_lent_ptr get_execution_context() = 0;
	virtual executor_context_const_lent_ptr get_execution_context() const = 0;
};

using async_base_dist_ref = distributed_reference_wrapper<async_interface_base>;
using async_base_const_dist_ref = distributed_reference_wrapper<const async_interface_base>;
using async_base_dist_ptr = distributed_pointer_wrapper<async_interface_base>;
using async_base_const_dist_ptr = distributed_pointer_wrapper<const async_interface_base>;

using async_base_lent_ref = lent_reference_wrapper<async_interface_base>;
using async_base_const_lent_ref = lent_reference_wrapper<const async_interface_base>;
using async_base_lent_ptr = lent_pointer_wrapper<async_interface_base>;
using async_base_const_lent_ptr = lent_pointer_wrapper<const async_interface_base>;

class async_cancellable_interface : public async_interface_base
{
public:
	enum CancelErrorCode
	{
		CancelNoAsync,
		CancelNoCallable,
		CancelAlreadyExecuted
	};
	typedef result<void,CancelErrorCode> cancel_result;

	virtual cancel_result cancel() = 0;
};

using async_cancellable_dist_ref = distributed_reference_wrapper<async_cancellable_interface>;
using async_cancellable_const_dist_ref = distributed_reference_wrapper<const async_cancellable_interface>;
using async_cancellable_dist_ptr = distributed_pointer_wrapper<async_cancellable_interface>;
using async_cancellable_const_dist_ptr = distributed_pointer_wrapper<const async_cancellable_interface>;

template<typename Return>
class async_execute_interface
{
public:
	enum StartErrorCode
	{
		NotAvailable,
		AlreadyStarted,
		AlreadyDone
	};
	typedef result<void,StartErrorCode> start_result;

	virtual start_result execute() = 0;
	virtual bool empty() const = 0;
};

template<typename Return>
using async_execute_dist_ref = distributed_reference_wrapper<async_execute_interface<Return>>;
template<typename Return>
using async_execute_const_dist_ref = distributed_reference_wrapper<const async_execute_interface<Return>>;
template<typename Return>
using async_execute_dist_ptr = distributed_pointer_wrapper<async_execute_interface<Return>>;
template<typename Return>
using async_execute_const_dist_ptr = distributed_pointer_wrapper<const async_execute_interface<Return>>;

template<typename Return>
using async_execute_lent_ref = lent_reference_wrapper<async_execute_interface<Return>>;
template<typename Return>
using async_execute_const_lent_ref = lent_reference_wrapper<const async_execute_interface<Return>>;
template<typename Return>
using async_execute_lent_ptr = lent_pointer_wrapper<async_execute_interface<Return>>;
template<typename Return>
using async_execute_const_lent_ptr = lent_pointer_wrapper<const async_execute_interface<Return>>;

}
