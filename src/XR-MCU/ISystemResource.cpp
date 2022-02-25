#include "ISystemResource.h"
#include "SystemResourceVisitor.h"

namespace mcu
{
namespace detail
{

void IRegisteredSystemResource::visit(const SystemResourceVisitor& i_visitor) const
{
	i_visitor(*this);
}
void IRegisteredSystemResource::visit(SystemResourceVisitor& i_visitor) const
{
	i_visitor(*this);
}

}
}