#pragma once

#include "IMCUProfiler.h"
#include "ddk_lend_from_this.h"
#include "ddk_thread_executor.h"

namespace pfr
{

class Profiler : public detail::IProfilerEx, public ddk::lend_from_this<Profiler,IProfiler>
{
	friend class IProfiler;

public:
	~Profiler();

private:
	Profiler();

	bool register_module(profile_module_id i_id) override;
	bool unregister_module_id(profile_module_id) override;
	measure_id register_measure(profile_module_id i_moduleId, imeasurable_unique_ref i_measurable) override;
	void publish_measure(profile_module_id i_moduleId, const Measure&) const override;
	void unregister_measure(profile_module_id i_moduleId, measure_id i_measureId) override;
	ddk::detail::connection_base& listen_measure(profile_module_id i_moduleId, const ddk::function<void(const Measure&)>&) override;

	void update();

	std::unordered_map<profile_module_id,Measurer> m_measurers;
	ddk::thread_polling_executor m_profileExecutor;
};

}