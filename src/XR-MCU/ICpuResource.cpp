#include "ICpuResource.h"
#include "SystemResourceVisitor.h"

namespace mcu
{
namespace detail
{

void IRegisteredCpuResource::visit(const SystemResourceVisitor& i_visitor) const
{
	i_visitor(*this);
}
void IRegisteredCpuResource::visit(SystemResourceVisitor& i_visitor) const
{
	i_visitor(*this);
}

}
}