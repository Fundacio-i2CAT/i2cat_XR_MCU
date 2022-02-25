#pragma once

#include "ddk_iterable_type_resolver.h"
#include "ddk_iterable_adaptor.h"
#include "ddk_iterable_adaptor_concepts.h"

namespace ddk
{
namespace detail
{

template<typename,typename>
struct iterable_adaptor_correspondence_resolver;

template<typename Iterable>
struct iterable_adaptor_correspondence_resolver<Iterable,const_forward_iterable_type>
{
	typedef forward_iterable_adaptor<const Iterable> type;
};
template<typename Iterable>
struct iterable_adaptor_correspondence_resolver<Iterable,forward_iterable_type>
{
	typedef forward_iterable_adaptor<Iterable> type;
};
template<typename Iterable>
struct iterable_adaptor_correspondence_resolver<Iterable,const_bidirectional_iterable_type>
{
    typedef bidirectional_iterable_adaptor<const Iterable> type;
};
template<typename Iterable>
struct iterable_adaptor_correspondence_resolver<Iterable,bidirectional_iterable_type>
{
    typedef bidirectional_iterable_adaptor<Iterable> type;
};
template<typename Iterable>
struct iterable_adaptor_correspondence_resolver<Iterable,const_random_access_iterable_type>
{
    typedef random_access_iterable_adaptor<const Iterable> type;
};
template<typename Iterable>
struct iterable_adaptor_correspondence_resolver<Iterable,random_access_iterable_type>
{
    typedef random_access_iterable_adaptor<Iterable> type;
};

template<typename Iterable, typename ... Types>
typename iterable_adaptor_correspondence_resolver<Iterable,iterable_type_correspondence<Iterable>>::type iterable_adaptor_resolver(Iterable&, Types ...);

template<typename Iterable>
using iterable_adaptor_correspondence = decltype(iterable_adaptor_resolver(std::declval<Iterable&>()));

TEMPLATE(typename Adaptor)
REQUIRES(IS_NON_CONST_EXCLUSIVE_FORWARD_ADAPTOR(Adaptor))
forward_iterable_type iterable_adaptor_type_resolver(Adaptor&, ...);

TEMPLATE(typename Adaptor)
REQUIRES(IS_NON_CONST_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR(Adaptor))
bidirectional_iterable_type iterable_adaptor_type_resolver(Adaptor&, ...);

TEMPLATE(typename Adaptor)
REQUIRES(IS_NON_CONST_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR(Adaptor))
random_access_iterable_type iterable_adaptor_type_resolver(Adaptor&, ...);

TEMPLATE(typename Adaptor)
REQUIRES(IS_CONST_EXCLUSIVE_FORWARD_ADAPTOR(Adaptor))
const_forward_iterable_type iterable_adaptor_type_resolver(Adaptor&, ...);

TEMPLATE(typename Adaptor)
REQUIRES(IS_CONST_EXCLUSIVE_BIDIRECTIONAL_ADAPTOR(Adaptor))
const_bidirectional_iterable_type iterable_adaptor_type_resolver(Adaptor&, ...);

TEMPLATE(typename Adaptor)
REQUIRES(IS_CONST_EXCLUSIVE_RANDOM_ACCESS_ADAPTOR(Adaptor))
const_random_access_iterable_type iterable_adaptor_type_resolver(Adaptor&, ...);

template<typename Iterable>
using iterable_adaptor_type_correspondence = decltype(iterable_adaptor_type_resolver(std::declval<iterable_adaptor<Iterable>&>()));

}

template<typename Iterable>
class iterable_adaptor : public detail::iterable_adaptor_correspondence<Iterable>
{
public:
    using detail::iterable_adaptor_correspondence<Iterable>::iterable_adaptor_correspondence;
};

}
