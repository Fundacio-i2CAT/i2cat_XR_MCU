#pragma once

#include "ddk_iaccess_provider.h"
#include "DecodedFrame.h"

namespace mcu
{
namespace detail
{

class IFusionDataProviderAccess
{
public:
	typedef std::vector<DecodedFrame> decoded_frame_container;
	typedef typename decoded_frame_container::iterator iterator;
	typedef typename decoded_frame_container::const_iterator const_iterator;
};

}

struct FusionDataProviderTraits
{
	typedef detail::IFusionDataProviderAccess provider_interface;
	typedef ddk::critical_section_context critical_context;
};

class IFusionDataProvider : public ddk::IAccessInterface<FusionDataProviderTraits>
{
protected:
	typedef detail::IFusionDataProviderAccess::decoded_frame_container decoded_frame_container;

public:
	typedef detail::IFusionDataProviderAccess::decoded_frame_container player_container_t;
	typedef detail::IFusionDataProviderAccess::iterator iterator;
	typedef detail::IFusionDataProviderAccess::const_iterator const_iterator;
	typedef FusionDataProviderTraits::critical_context critical_context;
	typedef ddk::critical_section<detail::IFusionDataProviderAccess> critical_section;
	typedef ddk::const_critical_section<detail::IFusionDataProviderAccess> const_critical_section;

	virtual ~IFusionDataProvider() = default;

};

}