#pragma once

#include "IProflierMeasurable.h"
#include "ProfilerDefs.h"
#include <unordered_map>
#include "IProfileMeasurer.h"
#include "ddk_lend_from_this.h"
#include "ddk_async_signal.h"

namespace pfr
{

class Measurer : public IMeasurer, public ddk::lend_from_this<Measurer,IMeasurer>
{
	typedef std::unordered_map<measure_id,imeasurable_unique_ref> measure_container;
	static const unsigned int k_measureTimeBaseLineInMS = 1000;

public:
	typedef measure_container::iterator iterator;
	typedef measure_container::const_iterator const_iterator;

	Measurer() = default;
	Measurer(measure_id i_id,imeasurable_unique_ref i_measure);
	Measurer(Measurer&& other);

	measure_id add_measurable(imeasurable_unique_ref i_measure);
	bool remove_measurable(measure_id i_id);
	void publish_measure(const Measure& i_measure) const;
	void measure(const profiler_time_point& i_time);
	ddk::detail::connection_base& listen_measure(const ddk::function<void(const Measure&)>& i_sink);
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	ddk::async_signal<void(const Measure&)> asig_onNewMeasure;

private:
	void onNewMeasure(const Measure&) const;

	measure_container m_measures;
	size_t m_currMeasureId = 0;
};

}