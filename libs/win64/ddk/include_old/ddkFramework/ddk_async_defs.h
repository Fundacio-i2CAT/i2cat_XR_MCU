#pragma once

#include "ddk_type_id.h"
#include "ddk_scoped_enum.h"

namespace ddk
{

struct sender_id_t;
typedef Id<size_t,sender_id_t> sender_id;

struct receiver_id_t;
typedef Id<size_t,receiver_id_t> receiver_id;

static const unsigned long k_messageQueueUpdateTimeInMS = 500;

}