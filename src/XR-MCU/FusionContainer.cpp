#include "FusionContainer.h"
#include "FusionDataContainer.h"

namespace mcu
{

FusionContainer::FusionContainer(size_t i_maxPendingFrames)
: m_maxPendingFrames(i_maxPendingFrames)
{
}
bool FusionContainer::register_frame_source(frame_source_id i_id, iphysical_object_const_lent_ref i_object)
{
	critical_context ctxt = enterWriter(ddk::Reentrancy::NON_REENTRANT);

	if (m_fusionCtr.find(i_id) == m_fusionCtr.end())
	{
		m_fusionCtr.emplace(std::piecewise_construct,std::forward_as_tuple(i_id),std::forward_as_tuple(i_id,m_maxPendingFrames,i_object,static_cast<const IFusionNotifier&>(*this)));

		leaveWriter(std::move(ctxt));

		return true;
	}
	else
	{
		leaveWriter(std::move(ctxt));

		return false;
	}
}
bool FusionContainer::unregister_frame_source(frame_source_id i_id)
{
	critical_context ctxt = enterWriter(ddk::Reentrancy::NON_REENTRANT);

	typename fusion_container::iterator itSource = m_fusionCtr.find(i_id);

	if (itSource != m_fusionCtr.end())
	{
		m_fusionCtr.erase(itSource);

		leaveWriter(std::move(ctxt));

		return true;
	}
	else
	{
		leaveWriter(std::move(ctxt));

		return false;
	}
}
void FusionContainer::add_decoded_frame(frame_source_id i_id, DecodedFrame i_frame)
{
	bool shallNotifyBoundingBoxChange = false;

	critical_context ctxt = enterWriter(ddk::Reentrancy::NON_REENTRANT);

	typename fusion_container::iterator itSource = m_fusionCtr.find(i_id);
	if (itSource != m_fusionCtr.end())
	{
		itSource->second.add_decoded_frame(std::move(i_frame));

		const bounding_box& otherBoundingBox = itSource->second.get_bounding_box();

		shallNotifyBoundingBoxChange = (m_boundingBox[i_id] != otherBoundingBox);

		m_boundingBox[i_id] = otherBoundingBox;
	}

	leaveWriter(std::move(ctxt));

	if(shallNotifyBoundingBoxChange)
	{
		sig_onBoundingBoxChanged.execute(i_id);
	}
}
FusionContainer::scene_info FusionContainer::get_current_scene_info(const frame_source_list_id& i_idList) const
{
	scene_info res;

	critical_context ctxt = enterReader(ddk::Reentrancy::NON_REENTRANT);

	if(i_idList.empty() == false)
	{
		typename fusion_container::const_iterator itSource = m_fusionCtr.begin();
		for(; itSource != m_fusionCtr.end(); ++itSource)
		{
			if(itSource->second.empty())
			{
				continue;
			}

			if(i_idList.find(itSource->first) != i_idList.end())
			{
				fusion_data_container::const_iterator itLast = itSource->second.last();

				res.push_back(std::make_pair(itSource->first,itLast->second.get_time_stamp()));
			}
		}
	}

	leaveReader(std::move(ctxt));

	return res;
}
FusionContainer::frame_collection FusionContainer::get_scene(const scene_info& i_sceneInfo) const
{
	frame_collection res;

	if(i_sceneInfo.empty() == false)
	{
		typename fusion_container::const_iterator itSource = m_fusionCtr.begin();
		for(; itSource != m_fusionCtr.end(); ++itSource)
		{
			if(itSource->second.empty())
			{
				continue;
			}

			if(std::find_if(i_sceneInfo.begin(),i_sceneInfo.end(),[&itSource](const std::pair<frame_source_id,mcu_frame_time_point>& i_pair) { return i_pair.first == itSource->first; }) != i_sceneInfo.end())
			{
				fusion_data_container::const_iterator itLast = itSource->second.last();

				res.push_back(std::make_pair(itSource->first,itLast));
			}
		}
	}

	return res;
}
FusionContainer::frame_collection FusionContainer::get_last_frame(const frame_source_list_id& i_idList) const
{
	frame_collection res;

	if(i_idList.empty() == false)
	{
		typename fusion_container::const_iterator itSource = m_fusionCtr.begin();
		for(;itSource!=m_fusionCtr.end();++itSource)
		{
			if (itSource->second.empty())
			{
				continue;
			}

			if (i_idList.find(itSource->first) != i_idList.end())
			{
				fusion_data_container::const_iterator itLast = itSource->second.last();

				res.push_back(std::make_pair(itSource->first,itLast));
			}
		}
	}

	return res;
}
bounding_box FusionContainer::get_bounding_box(const frame_source_id& i_id) const
{
	bounding_box res;

	critical_context ctxt = enterReader(ddk::Reentrancy::NON_REENTRANT);

	typename std::map<frame_source_id,bounding_box>::const_iterator itSource = m_boundingBox.find(i_id);
	if(itSource != m_boundingBox.end())
	{
		res = itSource->second;
	}

	leaveReader(std::move(ctxt));

	return res;
}
size_t FusionContainer::get_num_frame_sources() const
{
	return m_fusionCtr.size();
}
FusionContainer::critical_context FusionContainer::enterReader(ddk::Reentrancy i_reentrancy) const
{
	m_exclArea.enterReader(i_reentrancy);

	return ddk::context_acquired;
}
FusionContainer::critical_context FusionContainer::tryToEnterReader(ddk::Reentrancy i_reentrancy) const
{
	return (m_exclArea.tryToEnterReader(i_reentrancy)) ? ddk::context_acquired : ddk::context_unacquired;
}
void FusionContainer::leaveReader(critical_context) const
{
	m_exclArea.leaverReader();
}
FusionContainer::critical_context FusionContainer::enterWriter(ddk::Reentrancy i_reentrancy)
{
	m_exclArea.enterWriter(i_reentrancy);

	return ddk::context_acquired;
}
FusionContainer::critical_context FusionContainer::tryToEnterWriter(ddk::Reentrancy i_reentrancy)
{
	return (m_exclArea.tryToEnterWriter(i_reentrancy)) ? ddk::context_acquired : ddk::context_unacquired;
}
void FusionContainer::leaveWriter(critical_context)
{
	m_exclArea.leaveWriter();
}
void FusionContainer::onNewAvailableFrame() const
{
	sig_onNewDecodedFrame.execute();
}

}