#include "FusionModuleProfiler.h"
#include "IMCUProfiler.h"
#include "IFusionManager.h"

namespace mcu
{

const pfr::profile_module_id k_fusion_profile_module = pfr::detail::register_profiler_module<IFusionManager>();

}