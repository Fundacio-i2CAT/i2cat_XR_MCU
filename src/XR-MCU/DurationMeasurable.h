#pragma once

#include <chrono>
#include "ProfilerMeasurable.h"

namespace pfr
{

template<typename T, typename Ratio>
class DurationMeasurable : public Measurable<std::chrono::duration<int,Ratio>,T>
{
public:
	typedef std::chrono::duration<int,Ratio> value_type;

	DurationMeasurable(value_type& i_duration, profiler_time_duration i_timeBetweenMeasures = profiler_time_duration(0));

private:
	value_type private_measure(const profiler_time_point&) const override;

	value_type& m_duration;
};

}

#include "DurationMeasurable.inl"