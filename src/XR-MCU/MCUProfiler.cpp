#include "MCUProfiler.h"

namespace pfr
{

const std::chrono::milliseconds k_profilerUpdateTime = std::chrono::milliseconds(1000);

Profiler::Profiler()
{
	m_profileExecutor.set_update_time(k_profilerUpdateTime);

	m_profileExecutor.start_thread(ddk::make_function(this,&Profiler::update));
}
Profiler::~Profiler()
{
	m_profileExecutor.stop_thread();
}
void Profiler::update()
{
	const profiler_time_point profileTime = profiler_clock::now();

	std::unordered_map<profile_module_id,Measurer>::iterator itMeasurer = m_measurers.begin();
	for (;itMeasurer!=m_measurers.end();++itMeasurer)
	{
		itMeasurer->second.measure(profileTime);
	}
}
bool Profiler::register_module(profile_module_id i_moduleId)
{
	std::unordered_map<profile_module_id,Measurer>::iterator itMeasurer = m_measurers.find(i_moduleId);
	if (itMeasurer == m_measurers.end())
	{
		m_measurers.insert(std::make_pair(i_moduleId,Measurer{}));

		return true;
	}
	else
	{
		return false;
	}	
}
bool Profiler::unregister_module_id(profile_module_id i_moduleId)
{
	std::unordered_map<profile_module_id,Measurer>::iterator itMeasurer = m_measurers.find(i_moduleId);
	if (itMeasurer != m_measurers.end())
	{
		m_measurers.erase(itMeasurer);

		return true;
	}
	else
	{
		return false;
	}
}
measure_id Profiler::register_measure(profile_module_id i_moduleId, imeasurable_unique_ref i_measurable)
{
	std::unordered_map<profile_module_id,Measurer>::iterator itMeasurer = m_measurers.find(i_moduleId);
	if (itMeasurer != m_measurers.end())
	{
		return itMeasurer->second.add_measurable(std::move(i_measurable));
	}
	else
	{
		throw NonExistentProfileMdule{};
	}
}
void Profiler::publish_measure(profile_module_id i_moduleId, const Measure& i_measure) const
{
	std::unordered_map<profile_module_id,Measurer>::const_iterator itMeasurer = m_measurers.find(i_moduleId);
	if (itMeasurer != m_measurers.end())
	{
		itMeasurer->second.publish_measure(i_measure);
	}
}
void Profiler::unregister_measure(profile_module_id i_moduleId, measure_id i_measureId)
{
	std::unordered_map<profile_module_id,Measurer>::iterator itMeasurer = m_measurers.find(i_moduleId);
	if (itMeasurer != m_measurers.end())
	{
		if (itMeasurer->second.remove_measurable(i_measureId) == false)
		{
			throw NonExistentProfileMeasure{};
		}
	}
	else
	{
		throw NonExistentProfileMdule{};
	}
}
ddk::detail::connection_base& Profiler::listen_measure(profile_module_id i_moduleId, const ddk::function<void(const Measure&)>& i_sink)
{
	std::unordered_map<profile_module_id,Measurer>::iterator itMeasurer = m_measurers.find(i_moduleId);
	if (itMeasurer != m_measurers.end())
	{
		return itMeasurer->second.listen_measure(i_sink);
	}
	else
	{
		throw NonExistentProfileMdule{};
	}
}

}