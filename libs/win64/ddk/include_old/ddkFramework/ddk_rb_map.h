#pragma once

#include "ddk_map.h"
#include "ddk_rb_balancer.h"

namespace ddk
{
namespace detail
{

enum class MapColor : char
{
    Red,
    Black
};

template<typename Key,typename Value>
class rb_node : public map_node<Key,Value>
{
public:
    rb_node();
    template<typename ... Args>
    rb_node(const Key& i_key,Args&& ... i_args);
    rb_node(const rb_node& other) = default;
    rb_node(rb_node&& other) = default;
    rb_node& operator=(const rb_node& other) = default;

    inline unique_pointer_wrapper<rb_node<Key,Value>> replace_node(unique_reference_wrapper<rb_node<Key,Value>> other);
    template<typename Map>
    inline unique_reference_wrapper<rb_node<Key,Value>> extract_node(lent_pointer_wrapper<Map> i_map = nullptr);
    inline lent_pointer_wrapper<rb_node<Key,Value>> get_parent();
    inline lent_pointer_wrapper<rb_node<Key,Value>> get_left_child();
    inline unique_pointer_wrapper<rb_node<Key,Value>> extract_left_child();
    inline lent_pointer_wrapper<rb_node<Key,Value>> get_right_child();
    inline unique_pointer_wrapper<rb_node<Key,Value>> extract_right_child();
    inline void set_color(MapColor i_height);
    inline MapColor get_color() const;

    MapColor m_color = Red;
};

}

template<typename Key,typename Value,template<typename> class Allocator = typed_system_allocator>
using rb_map = detail::map_impl<Key,Value,detail::rb_node<Key,Value>,Allocator,detail::rb_balancer>;

}

#include "ddk_rb_map.inl"