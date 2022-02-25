#pragma once

#include "IProflierMeasurable.h"
#include <functional>

namespace pfr
{

template<typename Return, typename T>
class Measurable : public IMeasurable
{
public:
	typedef Return value_type;
	typedef typename std::add_lvalue_reference<value_type>::type reference;
	typedef typename std::add_lvalue_reference<typename std::add_const<value_type>::type>::type const_reference;

	Measurable(profiler_time_duration i_timeBetweenMeasures = 0);

	static measure_type get_type();

private:
	void bind(imeasurer_const_lent_ref i_measurer) override;
	void measure(const profiler_time_point& i_time) const override;
	
	virtual Return private_measure(const profiler_time_point&) const = 0;

	imeasurer_const_lent_ptr m_measurer;
	mutable profiler_time_point m_lastMeasure;
	profiler_time_duration m_timeBetweenMeasures;
};

}

#include "ProfilerMeasurable.inl"