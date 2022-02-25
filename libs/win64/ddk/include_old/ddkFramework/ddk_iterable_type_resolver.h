#pragma once

#include "ddk_iterable_type.h"
#include "ddk_iterable_concepts.h"

namespace ddk
{
namespace detail
{

TEMPLATE(typename Iterable)
REQUIRES(DYNAMIC_SIZE_CONTAINER(Iterable),IS_EXCLUSIVE_FORWARD_ITERABLE(Iterable))
forward_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

TEMPLATE(typename Iterable)
REQUIRES(DYNAMIC_SIZE_CONTAINER(Iterable),IS_EXCLUSIVE_BIDIRECTIONAL_ITERABLE(Iterable))
bidirectional_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

TEMPLATE(typename Iterable)
REQUIRES(DYNAMIC_SIZE_CONTAINER(Iterable),IS_EXCLUSIVE_RANDOM_ACCESS_ITERABLE(Iterable))
random_access_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

TEMPLATE(typename Iterable)
REQUIRES(FIXED_SIZE_CONTAINER(Iterable),IS_EXCLUSIVE_FORWARD_ITERABLE(Iterable))
const_forward_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

TEMPLATE(typename Iterable)
REQUIRES(FIXED_SIZE_CONTAINER(Iterable),IS_EXCLUSIVE_BIDIRECTIONAL_ITERABLE(Iterable))
const_bidirectional_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

TEMPLATE(typename Iterable)
REQUIRES(FIXED_SIZE_CONTAINER(Iterable),IS_EXCLUSIVE_RANDOM_ACCESS_ITERABLE(Iterable))
const_random_access_iterable_type iterable_type_correspondence_resolver(Iterable&, ...);

template<typename Iterable>
using iterable_type_correspondence = decltype(iterable_type_correspondence_resolver(std::declval<Iterable&>()));

}
}
