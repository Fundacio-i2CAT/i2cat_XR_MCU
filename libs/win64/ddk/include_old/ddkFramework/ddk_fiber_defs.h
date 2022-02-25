#pragma once

#include "ddk_type_id.h"
#include "ddk_scoped_enum.h"

namespace ddk
{

struct fiber_impl_t;
typedef Id<size_t,fiber_impl_t> fiber_id;

SCOPED_ENUM_DECL(FiberExecutionState,
	Idle,
	Executing,
	Cancelling,
	Done);

}