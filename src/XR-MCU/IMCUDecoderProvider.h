#pragma once

#include "ddkFramework/ddk_lent_reference_wrapper.h"

namespace mcu
{

class IDecoderProvider
{
public:
	virtual ~IDecoderProvider() = default;
};

typedef ddk::lent_reference_wrapper<IDecoderProvider> decoder_provider_lent_ref;
typedef ddk::lent_reference_wrapper<const IDecoderProvider> decoder_provider_const_lent_ref;

}