#include "IMCUProfiler.h"
#include "MCUProfiler.h"

namespace pfr
{

ddk::lent_reference_wrapper<IProfiler> IProfiler::get_instance()
{
	static Profiler s_profiler;

	return ddk::lend(s_profiler);
}

}