#pragma once

namespace ddk
{
namespace detail
{

template<typename Map,typename Node>
class rb_balancer
{
public:
    typedef rb_balancer<Map,Node> Balancer;

    inline static unique_reference_wrapper<Node> balance(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode);
};

}
}

#include "ddk_rb_balancer.inl"