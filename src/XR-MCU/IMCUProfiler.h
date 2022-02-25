#pragma once

#include "ddk_lent_reference_wrapper.h"
#include "ProfilerMeasurer.h"
#include "ProfilerDefs.h"

#define START_MEASURE(_MEASURER_ID,_MEASURE,_MEASURE_ID) \
	if(pfr::detail::iprofiler_ex_lent_ptr __profiler = ddk::static_lent_cast<pfr::detail::IProfilerEx>(pfr::IProfiler::get_instance())) \
	{ \
		_MEASURE_ID = __profiler->register_measure(_MEASURER_ID,_MEASURE); \
	} \
	else \
	{ \
		_MEASURE_ID = pfr::k_invalid_measure_id; \
	}
#define PUBLISH_MEASURE(_MEASURER_ID,_MEASURE_TYPE,TIME_BETWEEN_MEASURES,...) \
	{ \
		static std::map<typename std::remove_reference<decltype(TIME_BETWEEN_MEASURES.first)>::type,pfr::profiler_time_point> lastPublishedMeasureTime; \
		if((pfr::profiler_clock::now() - lastPublishedMeasureTime[TIME_BETWEEN_MEASURES.first]) > TIME_BETWEEN_MEASURES.second) \
		{ \
			lastPublishedMeasureTime[TIME_BETWEEN_MEASURES.first] = pfr::profiler_clock::now(); \
			if(pfr::detail::iprofiler_ex_lent_ptr __profiler = ddk::static_lent_cast<pfr::detail::IProfilerEx>(pfr::IProfiler::get_instance())) \
			{ \
				__profiler->publish_measure(_MEASURER_ID,pfr::Measure{_MEASURE_TYPE::get_type(),typename _MEASURE_TYPE::value_type(__VA_ARGS__)}); \
			} \
		} \
	}
#define STOP_MEASURE(_MEASURER_ID,_MEASURE_ID) \
	if(pfr::detail::iprofiler_ex_lent_ptr __profiler = ddk::static_lent_cast<pfr::detail::IProfilerEx>(pfr::IProfiler::get_instance())) \
	{ \
		__profiler->unregister_measure(_MEASURER_ID,_MEASURE_ID); \
		_MEASURE_ID = pfr::k_invalid_measure_id; \
	}

namespace pfr
{

class IProfiler
{
public:
	static ddk::lent_reference_wrapper<IProfiler> get_instance();

	virtual ~IProfiler() = default;

	template<typename T>
	ddk::detail::connection_base& listen_to(profile_module_id i_moduleId, const ddk::function<void(typename T::const_reference)>& i_sink)
	{
		static_assert(std::is_base_of<IMeasurable,T>::value, "You shall provider a measurable type");

		return listen_measure(i_moduleId,[i_sink](const Measure& i_measure){ if(i_measure.get_type() == T::get_type()) ddk::eval(i_sink,i_measure.template get<typename T::value_type>()); });
	}

private:
	virtual ddk::detail::connection_base& listen_measure(profile_module_id,const ddk::function<void(const Measure&)>&) = 0;
};

typedef ddk::lent_reference_wrapper<IProfiler> iprofiler_lent_ref;
typedef ddk::lent_reference_wrapper<const IProfiler> iprofiler_const_lent_ref;
typedef ddk::lent_pointer_wrapper<IProfiler> iprofiler_lent_ptr;
typedef ddk::lent_pointer_wrapper<const IProfiler> iprofiler_const_lent_ptr;

namespace detail
{

class IProfilerEx : public IProfiler
{
public:
	template<typename T>
	profile_module_id register_module()
	{
		const profile_module_id typeId = get_next_module_id<T>();

		return (register_module(typeId)) ? typeId : k_invalid_module_id;
	}
	virtual bool unregister_module_id(profile_module_id) = 0;
	virtual measure_id register_measure(profile_module_id, imeasurable_unique_ref) = 0;
	virtual void publish_measure(profile_module_id, const Measure&) const = 0;
	virtual void unregister_measure(profile_module_id, measure_id) = 0;

private:
	static profile_module_id _get_next_module_id()
	{
		static profile_module_id s_currId = 0;

		return profile_module_id(s_currId++);
	}
	template<typename>
	static profile_module_id get_next_module_id()
	{
		static profile_module_id s_typeId = _get_next_module_id();

		return s_typeId;
	}
	virtual bool register_module(profile_module_id) = 0;
};

typedef ddk::lent_reference_wrapper<IProfilerEx> iprofiler_ex_lent_ref;
typedef ddk::lent_reference_wrapper<const IProfilerEx> iprofiler_ex_const_lent_ref;
typedef ddk::lent_pointer_wrapper<IProfilerEx> iprofiler_ex_lent_ptr;
typedef ddk::lent_pointer_wrapper<const IProfilerEx> iprofiler_ex_const_lent_ptr;

template<typename T>
profile_module_id register_profiler_module()
{
	if(iprofiler_ex_lent_ptr __profiler = ddk::static_lent_cast<pfr::detail::IProfilerEx>(pfr::IProfiler::get_instance()))
	{
		return __profiler->register_module<T>();
	}
	else
	{
		return k_invalid_module_id;
	}
}

}

}