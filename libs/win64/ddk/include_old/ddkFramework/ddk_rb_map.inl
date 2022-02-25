
namespace ddk
{
namespace detail
{

template<typename Key,typename Value>
rb_node<Key,Value>::rb_node()
: cMapNode<T,R>()
, m_color(MapColor::Red)
{
}
template<typename Key,typename Value>
template<typename ... Args>
rb_node<Key,Value>::rb_node(const Key& key,Args&& ... i_args)
: map_node<Key,Value>(key,std::forward<Args>(i_args)...)
, m_color(MapColor::Red)
{
}
template<typename Key,typename Value>
unique_pointer_wrapper<rb_node<Key,Value>> rb_node<Key,Value>::replace_node(unique_reference_wrapper<rb_node<Key,Value>> other)
{
    return static_unique_cast<rb_node<Key,Value>>(map_node<Key,Value>::replace_node(std::move(other)));
}
template<typename Key,typename Value>
template<typename Map>
unique_reference_wrapper<rb_node<Key,Value>> rb_node<Key,Value>::extract_node(lent_pointer_wrapper<Map> i_map)
{
    return static_unique_cast<rb_node<Key,Value>>(map_node<Key,Value>::extract_node(i_map));
}
template<typename Key,typename Value>
lent_pointer_wrapper<rb_node<Key,Value>> rb_node<Key,Value>::get_parent()
{
    return static_lent_cast<rb_node<Key,Value>>(map_node<Key,Value>::get_parent());
}
template<typename Key,typename Value>
lent_pointer_wrapper<rb_node<Key,Value>> rb_node<Key,Value>::get_left_child()
{
    return static_lent_cast<rb_node<Key,Value>>(map_node<Key,Value>::get_left_child());
}
template<typename Key,typename Value>
unique_pointer_wrapper<rb_node<Key,Value>> rb_node<Key,Value>::extract_left_child()
{
    return static_unique_cast<rb_node<Key,Value>>(map_node<Key,Value>::extract_left_child());
}
template<typename Key,typename Value>
lent_pointer_wrapper<rb_node<Key,Value>> rb_node<Key,Value>::get_right_child()
{
    return static_lent_cast<rb_node<Key,Value>>(map_node<Key,Value>::get_right_child());
}
template<typename Key,typename Value>
unique_pointer_wrapper<rb_node<Key,Value>> rb_node<Key,Value>::extract_right_child()
{
    return static_unique_cast<rb_node<Key,Value>>(map_node<Key,Value>::extract_right_child());
}
template<typename Key,typename Value>
void rb_node<Key,Value>::set_color(MapColor i_color)
{
    m_color = i_color;
}
template<typename Key,typename Value>
MapColor rb_node<Key,Value>::get_color() const
{
    return m_color;
}

}
}