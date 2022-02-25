#pragma once

#include "ProfilerMeasurable.h"
#include "ddk_function.h"

namespace pfr
{

template<typename Return, typename T>
class FunctorMeasurable : public Measurable<Return,T>
{
public:
	FunctorMeasurable(const ddk::function<Return(const profiler_time_point&)>& i_functor, profiler_time_duration i_timeBetweenMeasures = profiler_time_duration(0));

private:
	Return private_measure(const profiler_time_point&) const override;

	const ddk::function<Return(const profiler_time_point&)> m_functor;
};

}

#include "FunctorMeasurable.inl"