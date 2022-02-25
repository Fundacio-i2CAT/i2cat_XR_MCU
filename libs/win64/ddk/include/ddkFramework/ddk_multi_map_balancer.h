#pragma once

#include "ddk_lent_reference_wrapper.h"
#include "ddk_unique_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename Map,typename Node>
class recursive_multi_map_balancer
{
public:
    typedef typename Map::balancer_t Balancer;

    inline static unique_reference_wrapper<Node> balance(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode);
};

template<typename Map,typename Node>
class multi_map_balancer
{
public:
    typedef typename Map::balancer_t Balancer;

    inline static unique_reference_wrapper<Node> balance(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode);
};

}

struct multi_balancer
{
	template<typename Map,typename Node>
	using Balancer = detail::multi_map_balancer<Map,Node>;
};

struct recursive_balancer
{
	template<typename Map,typename Node>
	using Balancer = detail::recursive_multi_map_balancer<Map,Node>;
};

}

#include "ddk_multi_map_balancer.inl"