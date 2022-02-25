#pragma once

#include "ddk_any_value.h"
#include "ProfilerDefs.h"

namespace pfr
{

class Measure
{
public:
	template<typename T>
	Measure(measure_type i_type, T&& i_value);

	template<typename T>
	const T& get() const;
	measure_type get_type() const;

private:
	measure_type m_type;
	ddk::any_value m_arena;
};

}

#include "ProfilerMeasure.inl"