#pragma once

#include "ddk_connection_base.h"

namespace ddk
{
namespace detail
{

class signal_connector
{
public:
	virtual void disconnect(const connection_base&) const = 0;
};

}
}