
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename IterableImpl, typename ... Args>
distributed_reference_wrapper<IterableImpl> make_iterable_impl(Args&& ... i_args)
{
    return make_distributed_reference<IterableImpl>(std::forward<Args>(i_args) ...);
}

}
}
