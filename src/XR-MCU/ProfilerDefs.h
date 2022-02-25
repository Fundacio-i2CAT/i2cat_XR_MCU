#pragma once

#include "ddk_type_id.h"
#include <chrono>

namespace pfr
{

struct ProfilerId_t;
typedef ddk::Id<size_t,ProfilerId_t> profiler_id;

extern const profiler_id k_invalidProfilerId;

struct MeasureType_t;
typedef ddk::Id<size_t,MeasureType_t> measure_type;

struct MeasureId_t;
typedef ddk::Id<size_t,MeasureId_t> measure_id;

extern const measure_id k_invalid_measure_id;

struct ProfileModuleId_t;
typedef size_t profile_module_id;

extern const profile_module_id k_invalid_module_id;

typedef std::chrono::steady_clock profiler_clock;
typedef profiler_clock::time_point profiler_time_point;
typedef profiler_clock::duration profiler_time_duration;

}
