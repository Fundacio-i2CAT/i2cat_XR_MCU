#pragma once

namespace ddk
{
namespace detail
{

template<typename Map,typename Node>
class avl_balancer
{
private:
    inline static void re_parent(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> node,unique_pointer_wrapper<Node> newNode);
    inline static void set_to_left(lent_reference_wrapper<Node> node,unique_pointer_wrapper<Node> newNode);
    inline static void set_to_right(lent_reference_wrapper<Node> node,unique_pointer_wrapper<Node> newNode);
    inline static lent_reference_wrapper<Node> balance_simple_to_left(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode);
    inline static lent_reference_wrapper<Node> balance_simple_to_right(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode);
    inline static lent_reference_wrapper<Node> balance_double_to_left(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode);
    inline static lent_reference_wrapper<Node> balance_double_to_right(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode);
    inline static unique_reference_wrapper<Node> balance(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode,bool heightIncrease);

public:
    typedef avl_balancer<Map,Node> Balancer;

    inline static unique_reference_wrapper<Node> balance(lent_reference_wrapper<Map> fromMap,lent_reference_wrapper<Node> fromNode);
};

}
}

#include "ddk_avl_balancer.inl"