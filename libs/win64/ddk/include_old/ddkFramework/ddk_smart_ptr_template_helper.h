#pragma once

#include "ddk_template_helper.h"
#include "ddk_unique_reference_wrapper.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_weak_pointer_wrapper.h"
#include "ddk_atomic_shared_reference_wrapper.h"
#include "ddk_atomic_weak_pointer_wrapper.h"

#define _IS_SMART_PTR(_NAME,_CLASS) \
template<typename> \
struct is_##_NAME; \
 \
template<typename T> \
struct is_##_NAME<_CLASS<T>> \
{ \
	static const bool value = true; \
}; \
template<typename> \
struct is_##_NAME \
{ \
	static const bool value = false; \
};

namespace ddk
{
namespace mpl
{

CONTAINS_SYMBOL(__shared_type_tag)
CONTAINS_SYMBOL(__distributed_type_tag)

_IS_SMART_PTR(lent_ptr,lent_pointer_wrapper)
_IS_SMART_PTR(lent_ref,lent_reference_wrapper)
_IS_SMART_PTR(unique_ptr,unique_pointer_wrapper)
_IS_SMART_PTR(unique_ref,unique_reference_wrapper)
_IS_SMART_PTR(shared_ptr,shared_pointer_wrapper)
_IS_SMART_PTR(shared_ref,shared_reference_wrapper)
_IS_SMART_PTR(dist_ptr,distributed_pointer_wrapper)
_IS_SMART_PTR(dist_ref,distributed_reference_wrapper)
_IS_SMART_PTR(weak_ptr,weak_pointer_wrapper)

}
}