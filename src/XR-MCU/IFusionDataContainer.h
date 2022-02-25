#pragma once

#include "IFusionDataProvider.h"

namespace mcu
{

class IFusionDataContainer : public IFusionDataProvider
{
public:
	virtual void add_decoded_frame(DecodedFrame i_frame) = 0;
};

typedef ddk::unique_reference_wrapper<IFusionDataContainer> fusion_data_container_unique_ref;
typedef ddk::unique_reference_wrapper<const IFusionDataContainer> fusion_data_container_const_unique_ref;
typedef ddk::unique_pointer_wrapper<IFusionDataContainer> fusion_data_container_unique_ptr;
typedef ddk::unique_pointer_wrapper<const IFusionDataContainer> fusion_data_container_const_unique_ptr;
typedef ddk::lent_reference_wrapper<IFusionDataContainer> fusion_data_container_lent_ref;
typedef ddk::lent_reference_wrapper<const IFusionDataContainer> fusion_data_container_const_lent_ref;
typedef ddk::lent_pointer_wrapper<IFusionDataContainer> fusion_data_container_lent_ptr;
typedef ddk::lent_pointer_wrapper<const IFusionDataContainer> fusion_data_container_const_lent_ptr;

}