#include "SystemResourceVisitor.h"

namespace mcu
{

void SystemResourceVisitor::operator()(const detail::IRegisteredSystemResource& i_resource)
{
	//do nothing
}
void SystemResourceVisitor::operator()(const detail::IRegisteredSystemResource& i_resource) const
{
	//do nothing
}
void SystemResourceVisitor::operator()(const detail::IRegisteredCpuResource& i_resource)
{
	//do nothing
}
void SystemResourceVisitor::operator()(const detail::IRegisteredCpuResource& i_resource) const
{
	//do nothing
}

}