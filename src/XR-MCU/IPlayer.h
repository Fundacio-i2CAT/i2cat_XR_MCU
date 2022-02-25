#pragma once

#include "ddkFramework/ddk_unique_reference_wrapper.h"
#include "ddkFramework/ddk_lent_reference_wrapper.h"
#include "PlayerDefs.h"
#include "IRawFrameSource.h"
#include "ddk_result.h"
#include "IPhysicalObject.h"

namespace mcu
{

class IPlayer : public IPhysicalObject, public IRawFrameSource
{
public:
	virtual ~IPlayer() = default;
	virtual void start() = 0;
	virtual void update(const PlayerUpdateMessage&) = 0;
	virtual void notify() const = 0;
	virtual void stop() = 0;
};

typedef ddk::unique_reference_wrapper<IPlayer> iplayer_unique_ref;
typedef ddk::unique_reference_wrapper<const IPlayer> iplayer_const_unique_ref;
typedef ddk::lent_reference_wrapper<IPlayer> iplayer_lent_ref;
typedef ddk::lent_reference_wrapper<const IPlayer> iplayer_const_lent_ref;

}