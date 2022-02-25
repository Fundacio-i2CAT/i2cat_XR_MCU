#pragma once

#include "ddk_iaccess_provider.h"
#include "ddk_critical_section_context.h"
#include "IPlayer.h"
#include "ddk_transformed_bidirectional_iterator.h"
#include "ddk_result.h"
#include <map>

namespace mcu
{
namespace detail
{

class IPlayerContainerAccess
{
public:
	typedef std::map<player_id,iplayer_unique_ref> player_container;
	typedef typename player_container::iterator private_iterator;
	typedef typename player_container::const_iterator private_const_iterator;
	typedef ddk::TransformedBidirectionalIterator<iplayer_lent_ref> iterator;
	typedef ddk::TransformedBidirectionalIterator<iplayer_const_lent_ref> const_iterator;

	//const impl
	virtual const_iterator begin() const = 0;
	virtual const_iterator end() const = 0;
	virtual const_iterator find(player_id) const = 0;

	//non const impl
	virtual iterator begin() = 0;
	virtual iterator end() = 0;
	virtual iterator find(player_id) = 0;
};

}

struct PlayerContainerTraits
{
	typedef detail::IPlayerContainerAccess provider_interface;
	typedef ddk::critical_section_context critical_context;
};

class IPlayerProvider : public ddk::IAccessInterface<PlayerContainerTraits>
{
public:
	typedef detail::IPlayerContainerAccess::player_container player_container;
	typedef detail::IPlayerContainerAccess::iterator iterator;
	typedef detail::IPlayerContainerAccess::const_iterator const_iterator;
	typedef ddk::IAccessInterface<PlayerContainerTraits>::critical_context critical_context;
	typedef ddk::critical_section<PlayerContainerTraits> critical_section;
	typedef ddk::const_critical_section<PlayerContainerTraits> const_critical_section;

	virtual ~IPlayerProvider() = default;
};

typedef ddk::unique_reference_wrapper<IPlayerProvider> player_provider_unique_ref;
typedef ddk::unique_reference_wrapper<const IPlayerProvider> player_provider_const_unique_ref;
typedef ddk::unique_pointer_wrapper<IPlayerProvider> player_provider_unique_ptr;
typedef ddk::unique_pointer_wrapper<const IPlayerProvider> player_provider_const_unique_ptr;
typedef ddk::lent_reference_wrapper<IPlayerProvider> player_provider_lent_ref;
typedef ddk::lent_reference_wrapper<const IPlayerProvider> player_provider_const_lent_ref;
typedef ddk::lent_pointer_wrapper<IPlayerProvider> player_provider_lent_ptr;
typedef ddk::lent_pointer_wrapper<const IPlayerProvider> player_provider_const_lent_ptr;

}