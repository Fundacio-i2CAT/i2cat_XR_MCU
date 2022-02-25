#include "CwipcFusionedFrame.h"
#include "cwipc_util/api.h"
#include "cwipc/include/cwipc_codec/api.h"

namespace mcu {
	namespace detail {

		bool FrameIdComparator::operator()(const frame_source_id& i_lhs, const frame_source_id& i_rhs) const {
			return i_lhs > i_rhs;
		}

	}

	CwipcFusionedFrame::CwipcFusionedFrame(cwipc* i_data) : CwipcDecodedFrame(i_data) {}

	cwipc* CwipcFusionedFrame::getData() {
		return CwipcDecodedFrame::getData();
	}

	const cwipc* CwipcFusionedFrame::getData() const {
		return CwipcDecodedFrame::getData();
	}

	bool CwipcFusionedFrame::merge(const IDecodedFrameImpl* i_frame, frame_source_id i_id, const mcu_frame_time_point& i_timeStamp)
	{
		if (const CwipcDecodedFrame* frameToMerge = static_cast<const CwipcDecodedFrame*>(i_frame))
		{
			if(cwipc* dataToMerge = const_cast<CwipcDecodedFrame*>(frameToMerge)->getData())
			{
				if (cwipc* mergedData = getData())
				{
					float currentCellSize = mergedData->cellsize();
					float toMergeCellSize = dataToMerge->cellsize(); 
					if (toMergeCellSize >= currentCellSize) mergedData->_set_cellsize(toMergeCellSize);
					size_t framePos = 0;
					bool newestFrame = false;

					if (applies_to_scene(i_id, i_timeStamp, framePos, newestFrame))
					{
						cwipc* sampledFrame = dataToMerge;

						cwipc_pcl_pointcloud mergedPC = mergedData->access_pcl_pointcloud();
						if(cwipc_pcl_pointcloud pcToMerge = const_cast<cwipc*>(sampledFrame)->access_pcl_pointcloud())
						{
							const size_t mergedPCSize = mergedPC->size();
							auto& mergedPoints = mergedPC->points;
							const auto& pointsToMerge = pcToMerge->points;

							const size_t pcToMergeSize = pcToMerge->size();

							if(newestFrame)
							{
								int difference = pcToMergeSize - m_frameInfo[i_id].m_size;
								Shift(framePos,difference);
							}

							mergedPoints.resize(framePos + pcToMergeSize);

							memcpy(&mergedPoints[framePos],&pointsToMerge[0],sizeof(cwipc_pcl_point) * pcToMergeSize);

							if(sampledFrame != dataToMerge)
							{
								sampledFrame->free();
							}

							m_frameInfo[i_id] = frame_info{ framePos, pcToMergeSize, i_timeStamp };

							return true;
						}
					}
				}
			}
		}

		return false;
	}
	void CwipcFusionedFrame::visit(FusionedFrameVisitor& i_visitor)
	{
		i_visitor.visit(*this);
	}

	bool CwipcFusionedFrame::applies_to_scene(frame_source_id i_id, const mcu_frame_time_point& i_timeStamp, size_t& o_pos, bool& isNew) {
		std::map<frame_source_id, frame_info>::iterator itFrameInfo = m_frameInfo.lower_bound(i_id);

		if (itFrameInfo == m_frameInfo.end()) { // First PC to Merge
			o_pos = 0;
		} 
		else if (itFrameInfo->first != i_id) { // Different ID to merge
			o_pos = itFrameInfo->second.m_initPos + itFrameInfo->second.m_size;
		} 
		else if (itFrameInfo->second.m_timeStamp < i_timeStamp) { // Same ID but with newer PC
			o_pos = itFrameInfo->second.m_initPos;
			isNew = itFrameInfo->second.m_timeStamp < i_timeStamp;
		} 
		else {
			return false;
		}

		return true;
	}

	void CwipcFusionedFrame::Shift(size_t o_pos, int diff) {
		std::map<frame_source_id, frame_info>::iterator itFrameInfo;

		for (itFrameInfo = m_frameInfo.begin(); itFrameInfo != m_frameInfo.end(); itFrameInfo++) {
			if (itFrameInfo->second.m_initPos > o_pos) {
				itFrameInfo->second.m_initPos += diff;
			}
		}
	}

}