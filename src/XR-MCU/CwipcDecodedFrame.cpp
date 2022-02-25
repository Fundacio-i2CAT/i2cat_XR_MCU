#include "CwipcDecodedFrame.h"
#include "cwipc_util/api.h"
#include <utility>
#include "DecodedFrameVisitor.h"
#include "cwipc/include/cwipc_codec/api.h"

namespace mcu
{

CwipcDecodedFrame::CwipcDecodedFrame(cwipc* i_data)
: m_data(i_data)
{
	DDK_ASSERT(m_data!=nullptr, "Initializing cwi frame with null data");
}
CwipcDecodedFrame::~CwipcDecodedFrame()
{
	if (m_data)
	{
		m_data->free();
	}
}
cwipc* CwipcDecodedFrame::getData()
{
	return m_data;
}
const cwipc* CwipcDecodedFrame::getData() const
{
	return m_data;
}
DecodecType CwipcDecodedFrame::get_type() const
{
	return DecodecType::Cwipc;
}
void CwipcDecodedFrame::sample(float i_voxelSize)
{
	if (cwipc* data = m_data)
	{
		m_data = cwipc_downsample(data, i_voxelSize);

		if (data != m_data)
		{
			data->free();
		}
	}
}
void CwipcDecodedFrame::resize(size_t i_size)
{
	if (m_data)
	{
		m_data->access_pcl_pointcloud()->points.reserve(i_size);
	}
}
size_t CwipcDecodedFrame::size() const
{
	return (m_data) ? m_data->count() : 0;
}
bool CwipcDecodedFrame::empty() const
{
	return (m_data) ? (m_data->count() == 0) : true;
}
CwipcDecodedFrame::const_iterator CwipcDecodedFrame::begin() const
{
	return m_data->access_pcl_pointcloud()->begin();
}
CwipcDecodedFrame::const_iterator CwipcDecodedFrame::end() const
{
	return m_data->access_pcl_pointcloud()->end();
}
CwipcDecodedFrame::iterator CwipcDecodedFrame::begin()
{
	return m_data->access_pcl_pointcloud()->begin();
}
CwipcDecodedFrame::iterator CwipcDecodedFrame::end()
{
	return m_data->access_pcl_pointcloud()->end();
}
void CwipcDecodedFrame::visit(DecodedFrameVisitor& i_visitor)
{
	if (m_data)
	{
		i_visitor.visit(*this);
	}
}

}