#pragma once

namespace mcu
{

class IFusionNotifier
{
public:
	virtual ~IFusionNotifier() = default;

	virtual void onNewAvailableFrame() const = 0;
};

}