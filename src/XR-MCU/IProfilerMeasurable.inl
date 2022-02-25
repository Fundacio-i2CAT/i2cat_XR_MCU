
#include "ddk_reference_wrapper.h"

namespace pfr
{

template<typename T, typename ... Args>
imeasurable_unique_ref make_measurable(Args&& ... i_args)
{
	return ddk::make_unique_reference<T>(std::forward<Args>(i_args) ...);
}

}