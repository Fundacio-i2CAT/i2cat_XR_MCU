#pragma once

#include "DecodedFrame.h"
#include "cwipc_util/api_pcl.h"

namespace mcu
{

class CwipcDecodedFrame : public detail::DecodedFrameImpl
{
public:
	typedef pcl::PointCloud<cwipc_pcl_point>::iterator iterator;
	typedef pcl::PointCloud<cwipc_pcl_point>::const_iterator const_iterator;

	CwipcDecodedFrame(cwipc* i_data);
	CwipcDecodedFrame(const CwipcDecodedFrame&) = delete;
	CwipcDecodedFrame(CwipcDecodedFrame&&) = delete;
	~CwipcDecodedFrame();

	CwipcDecodedFrame& operator=(const CwipcDecodedFrame&) = delete;
	CwipcDecodedFrame& operator=(CwipcDecodedFrame&&) = delete;
	cwipc* getData();
	const cwipc* getData() const;
	const_iterator begin() const;
	const_iterator end() const;
	iterator begin();
	iterator end();

private:
	DecodecType get_type() const override;
	void visit(DecodedFrameVisitor&) override;
	void sample(float i_voxelSize) override;
	void resize(size_t i_size) override;
	size_t size() const override;
	bool empty() const override;

	cwipc* m_data;
};

}