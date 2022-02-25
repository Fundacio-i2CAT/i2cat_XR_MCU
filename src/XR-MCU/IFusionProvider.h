#pragma once

#include "FusionDataContainer.h"
#include "FrameDefs.h"
#include "FusionPlayerDataContainer.h"
#include "IFusionNotifier.h"
#include "PlayerUtils.h"
#include "ddk_signal.h"
#include "ddk_iaccess_provider.h"
#include <map>

namespace mcu
{
namespace detail
{

class IFusionProviderAccess
{
public:
	typedef FusionPlayerDataContainer fusion_data_container;
	typedef std::map<frame_source_id,fusion_data_container> fusion_container;
	typedef std::vector<std::pair<frame_source_id,FusionDataContainer<DecodedFrame>::const_iterator>> frame_collection;
	typedef typename frame_collection::iterator frame_collection_iterator;
	typedef std::vector<std::pair<frame_source_id,mcu_frame_time_point>> scene_info;

	virtual frame_collection get_last_frame(const frame_source_list_id&) const = 0;
	virtual frame_collection get_scene(const scene_info& i_sceneInfo) const = 0;
	virtual size_t get_num_frame_sources() const = 0;
};

}

struct FusionProviderTraits
{
	typedef detail::IFusionProviderAccess provider_interface;
	typedef ddk::critical_section_context critical_context;
};

class IFusionProvider : public ddk::IAccessInterface<FusionProviderTraits>, protected IFusionNotifier
{
protected:
	typedef detail::IFusionProviderAccess::fusion_data_container fusion_data_container;
	typedef detail::IFusionProviderAccess::fusion_container fusion_container;
public:
	typedef detail::IFusionProviderAccess::frame_collection frame_collection;
	typedef detail::IFusionProviderAccess::frame_collection_iterator frame_collection_iterator;
	typedef FusionProviderTraits::critical_context critical_context;
	typedef ddk::critical_section<FusionProviderTraits> critical_section;
	typedef ddk::const_critical_section<FusionProviderTraits> const_critical_section;
	typedef detail::IFusionProviderAccess::scene_info scene_info;
	typedef typename scene_info::iterator scene_iterator;
	typedef typename scene_info::const_iterator scene_const_iterator;

	virtual ~IFusionProvider() = default;
	virtual scene_info get_current_scene_info(const frame_source_list_id&) const = 0;
	virtual bounding_box get_bounding_box(const frame_source_id&) const = 0;

	ddk::signal<void(frame_source_id)> sig_onBoundingBoxChanged;
	ddk::signal<void()> sig_onNewDecodedFrame;	
};

}