#pragma once

#include "ProfilerMeasurable.h"

namespace pfr
{

template<typename T>
class OperationsRateMeasurable : public Measurable<float,T>
{
public:
	OperationsRateMeasurable(size_t& i_numFrameSinceLastEpoch, profiler_time_duration i_timeBetweenMeasures = profiler_time_duration(0));

private:
	float private_measure(const profiler_time_point&) const override;

	size_t& m_lastFramesSinceLastEpoch;
	mutable profiler_time_point m_lastUpdateTime;
};

}

#include "OperationsRateMeasurable.inl"