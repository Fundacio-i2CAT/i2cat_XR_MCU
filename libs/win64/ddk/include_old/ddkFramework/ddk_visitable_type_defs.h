#pragma once

#include "ddk_type_id.h"
#include "ddk_rtti_defs.h"

namespace ddk
{

struct visitable_type_t;
typedef Id<std::pair<rtti::TypeInfo,rtti::TypeInfo>,visitable_type_t> visitable_type_info;

}
