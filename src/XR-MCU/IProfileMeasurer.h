#pragma once

#include "ProfilerMeasure.h"
#include "ddk_lent_reference_wrapper.h"

namespace pfr
{

class IMeasurer
{
public:
	virtual ~IMeasurer() = default;

	virtual void onNewMeasure(const Measure&) const = 0;
};

typedef ddk::lent_reference_wrapper<IMeasurer> imeasurer_lent_ref;
typedef ddk::lent_reference_wrapper<const IMeasurer> imeasurer_const_lent_ref;
typedef ddk::lent_pointer_wrapper<IMeasurer> imeasurer_lent_ptr;
typedef ddk::lent_pointer_wrapper<const IMeasurer> imeasurer_const_lent_ptr;

}