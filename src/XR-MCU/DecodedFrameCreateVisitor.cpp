#include "DecodedFrameCreateVisitor.h"

namespace mcu
{

FusionedFrameCreateVisitor::FusionedFrameCreateVisitor(mcu_frame_time_point i_timeStamp)
: m_timeStamp(i_timeStamp)
{
}
detail::IFusionedFrameImpl* FusionedFrameCreateVisitor::extract_impl()
{
	detail::IFusionedFrameImpl* res = nullptr;

	std::swap(res,m_impl);

	return res;
}
void FusionedFrameCreateVisitor::visit(CwipcDecodedFrame& i_frame)
{
	m_impl = new CwipcFusionedFrame(cwipc_from_pcl(new_cwipc_pcl_pointcloud(),std::chrono::time_point_cast<std::chrono::milliseconds>(m_timeStamp).time_since_epoch().count(), NULL, CWIPC_API_VERSION));
}
void FusionedFrameCreateVisitor::visit(CwipcFusionedFrame& i_frame)
{
	if(cwipc_pcl_pointcloud pc = i_frame.getData()->access_pcl_pointcloud())
	{
		pc->clear();

		m_impl = new CwipcFusionedFrame(cwipc_from_pcl(pc,std::chrono::time_point_cast<std::chrono::milliseconds>(m_timeStamp).time_since_epoch().count(),NULL,CWIPC_API_VERSION));
	}
}

}