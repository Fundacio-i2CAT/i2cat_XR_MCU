#pragma once

#include "ProfilerMeasure.h"
#include "ddk_signal.h"
#include "ProfilerDefs.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "IProfileMeasurer.h"

namespace pfr
{

class IMeasurable
{
public:
	virtual ~IMeasurable() = default;

	virtual void bind(imeasurer_const_lent_ref i_measurer) = 0;
	virtual void measure(const profiler_time_point&) const = 0;
};

typedef ddk::unique_reference_wrapper<IMeasurable> imeasurable_unique_ref;
typedef ddk::unique_reference_wrapper<const IMeasurable> imeasurable_const_unique_ref;
typedef ddk::lent_reference_wrapper<IMeasurable> imeasurable_lent_ref;
typedef ddk::lent_reference_wrapper<const IMeasurable> imeasurable_const_lent_ref;

template<typename T, typename ... Args>
imeasurable_unique_ref make_measurable(Args&& ... i_args);

}

#include "IProfilerMeasurable.inl"