#pragma once

#include "IMCUDecoderProvider.h"
#include "ddkFramework/ddk_lend_from_this.h"

namespace mcu
{

class MCUDecoderBank : public IDecoderProvider, private ddk::lend_from_this<MCUDecoderBank,IDecoderProvider>
{



};

}