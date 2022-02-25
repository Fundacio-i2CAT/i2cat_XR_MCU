#pragma once

#include "ddk_iterable_state.h"
#include "ddk_iterable_action_state.h"

namespace ddk
{
namespace detail
{

class iterable_interface
{
public:
    virtual iterable_state& get_state() = 0;
    virtual const iterable_state& get_state() const = 0;
    virtual action_state_lent_ref get_action_state() = 0;
    virtual action_state_const_lent_ref get_action_state() const = 0;

protected:
    virtual ~iterable_interface() = default;
};

}
}