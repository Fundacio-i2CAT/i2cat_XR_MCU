#include "ProfilerMeasurer.h"

namespace pfr
{

Measurer::Measurer(measure_id i_id, imeasurable_unique_ref i_measure)
: asig_onNewMeasure(std::chrono::milliseconds(k_measureTimeBaseLineInMS))
{
	m_measures.insert(std::make_pair(i_id,std::move(i_measure)));
}
Measurer::Measurer(Measurer&& other)
: m_measures(std::move(other.m_measures))
, m_currMeasureId(other.m_currMeasureId)
, asig_onNewMeasure(std::chrono::milliseconds(k_measureTimeBaseLineInMS))
{
}
measure_id Measurer::add_measurable(imeasurable_unique_ref i_measure)
{
	std::pair<iterator,bool> insertRes = m_measures.insert(std::make_pair(m_currMeasureId,std::move(i_measure)));

	if (insertRes.second)
	{
		insertRes.first->second->bind(ddk::lend(*this));

		return measure_id(m_currMeasureId++);
	}
	else
	{
		return k_invalid_measure_id;
	}
}
bool Measurer::remove_measurable(measure_id i_id)
{
	iterator itMeasure = m_measures.find(i_id);

	if (itMeasure != m_measures.end())
	{
		m_measures.erase(itMeasure);

		return true;
	}
	else
	{
		return false;
	}
}
void Measurer::publish_measure(const Measure& i_measure) const
{
	asig_onNewMeasure.execute(i_measure);
}
ddk::detail::connection_base& Measurer::listen_measure(const ddk::function<void(const Measure&)>& i_sink)
{
	return asig_onNewMeasure.connect(i_sink);
}
void Measurer::measure(const profiler_time_point& i_time)
{
	measure_container::iterator itMeasure = m_measures.begin();
	for(;itMeasure!=m_measures.end();++itMeasure)
	{
		itMeasure->second->measure(i_time);
	}
}
Measurer::iterator Measurer::begin()
{
	return m_measures.begin();
}
Measurer::const_iterator Measurer::begin() const
{
	return m_measures.begin();
}
Measurer::iterator Measurer::end()
{
	return m_measures.end();
}
Measurer::const_iterator Measurer::end() const
{
	return m_measures.end();
}
void Measurer::onNewMeasure(const Measure& i_measure) const
{
	asig_onNewMeasure.execute(i_measure);
}

}