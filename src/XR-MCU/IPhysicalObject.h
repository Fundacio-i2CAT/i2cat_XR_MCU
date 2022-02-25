#pragma once

#include "PlayerTransformation.h"
#include "PlayerDefs.h"
#include "ddk_signal.h"

namespace mcu
{

class IPhysicalObject
{
public:
	virtual const transformation& get_transformation() const = 0;
	virtual Frustrum get_frustrum() const = 0;

	ddk::signal<void()> sig_onUpdate;
};

typedef ddk::lent_reference_wrapper<IPhysicalObject> iphysical_object_lent_ref;
typedef ddk::lent_reference_wrapper<const IPhysicalObject> iphysical_object_const_lent_ref;
typedef ddk::lent_pointer_wrapper<IPhysicalObject> iphysical_object_lent_ptr;
typedef ddk::lent_pointer_wrapper<const IPhysicalObject> iphysical_object_const_lent_ptr;

}