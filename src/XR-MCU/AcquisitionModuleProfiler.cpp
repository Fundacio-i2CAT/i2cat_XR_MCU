#include "AcquisitionModuleProfiler.h"
#include "IMCUProfiler.h"
#include "IRawFrameDecoder.h"

namespace mcu
{

const pfr::profile_module_id k_acquisition_profile_module = pfr::detail::register_profiler_module<IRawFrameDecoder>();

}