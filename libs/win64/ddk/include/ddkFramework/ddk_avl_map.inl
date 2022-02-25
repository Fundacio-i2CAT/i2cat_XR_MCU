
namespace ddk
{
namespace detail
{

template<typename Key,typename Value>
avl_node<Key,Value>::avl_node()
: map_node<Key,Value>()
, m_height(0)
{
}
template<typename Key,typename Value>
template<typename ... Args>
avl_node<Key,Value>::avl_node(const Key& key, Args&& ... i_args)
: map_node<Key,Value>(key,std::forward<Args>(i_args)...)
, m_height(0)
{
}
template<typename Key,typename Value>
unique_pointer_wrapper<avl_node<Key,Value>> avl_node<Key,Value>::replace_node(unique_reference_wrapper<avl_node<Key,Value>> other)
{
    return static_unique_cast<avl_node<Key,Value>>(map_node<Key,Value>::replace_node(std::move(other)));
}
template<typename Key,typename Value>
template<typename Map>
unique_reference_wrapper<avl_node<Key,Value>> avl_node<Key,Value>::extract_node(lent_pointer_wrapper<Map> i_map)
{
    return static_unique_cast<avl_node<Key,Value>>(map_node<Key,Value>::extract_node(i_map));
}
template<typename Key,typename Value>
lent_pointer_wrapper<avl_node<Key,Value>> avl_node<Key,Value>::get_parent()
{
    return static_lent_cast<avl_node<Key,Value>>(map_node<Key,Value>::get_parent());
}
template<typename Key,typename Value>
lent_pointer_wrapper<avl_node<Key,Value>> avl_node<Key,Value>::get_left_child()
{
    return static_lent_cast<avl_node<Key,Value>>(map_node<Key,Value>::get_left_child());
}
template<typename Key,typename Value>
lent_pointer_wrapper<const avl_node<Key,Value>> avl_node<Key,Value>::get_left_child() const
{
    return static_lent_cast<const avl_node<Key,Value>>(map_node<Key,Value>::get_left_child());
}
template<typename Key,typename Value>
unique_pointer_wrapper<avl_node<Key,Value>> avl_node<Key,Value>::extract_left_child()
{
    return static_unique_cast<avl_node<Key,Value>>(map_node<Key,Value>::extract_left_child());
}
template<typename Key,typename Value>
lent_pointer_wrapper<avl_node<Key,Value>> avl_node<Key,Value>::get_right_child()
{
    return static_lent_cast<avl_node<Key,Value>>(map_node<Key,Value>::get_right_child());
}
template<typename Key,typename Value>
lent_pointer_wrapper<const avl_node<Key,Value>> avl_node<Key,Value>::get_right_child() const
{
    return static_lent_cast<const avl_node<Key,Value>>(map_node<Key,Value>::get_right_child());
}
template<typename Key,typename Value>
unique_pointer_wrapper<avl_node<Key,Value>> avl_node<Key,Value>::extract_right_child()
{
    return static_unique_cast<avl_node<Key,Value>>(map_node<Key,Value>::extract_right_child());
}
template<typename Key,typename Value>
void avl_node<Key,Value>::set_height(signed short i_height)
{
    m_height = i_height;
}
template<typename Key,typename Value>
signed short avl_node<Key,Value>::get_height() const
{
    return m_height;
}

}
}