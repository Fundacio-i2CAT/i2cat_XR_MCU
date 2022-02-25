#pragma once

#include "ddk_map.h"
#include "ddk_avl_balancer.h"
#include "ddk_system_allocator.h"

namespace ddk
{
namespace detail
{

template<typename Key,typename Value>
class avl_node : public map_node<Key,Value>
{
public:
    avl_node();
    template<typename ... Args>
    avl_node(const Key& i_key, Args&& ... i_args);
    avl_node(const avl_node& other) = default;
    avl_node(avl_node&& other) = default;
    avl_node& operator=(const avl_node& other) = default;

    inline unique_pointer_wrapper<avl_node<Key,Value>> replace_node(unique_reference_wrapper<avl_node<Key,Value>> other);
    template<typename Map>
    inline unique_reference_wrapper<avl_node<Key,Value>> extract_node(lent_pointer_wrapper<Map> i_map = nullptr);
    inline lent_pointer_wrapper<avl_node<Key,Value>> get_parent();
    inline lent_pointer_wrapper<avl_node<Key,Value>> get_left_child();
    inline lent_pointer_wrapper<const avl_node<Key,Value>> get_left_child() const;
    inline unique_pointer_wrapper<avl_node<Key,Value>> extract_left_child();
    inline lent_pointer_wrapper<avl_node<Key,Value>> get_right_child();
    inline lent_pointer_wrapper<const avl_node<Key,Value>> get_right_child() const;
    inline unique_pointer_wrapper<avl_node<Key,Value>> extract_right_child();
    inline void set_height(signed short i_height);
    inline signed short get_height() const;

    signed short  m_height = 0;
};

}

template<typename Key,typename Value,template<typename> class Allocator = typed_system_allocator>
using avl_map = detail::map_impl<Key,Value,detail::avl_node<Key,Value>,Allocator,detail::avl_balancer>;

}

#include "ddk_avl_map.inl"