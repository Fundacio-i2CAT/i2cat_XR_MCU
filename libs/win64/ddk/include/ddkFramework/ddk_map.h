#pragma once

#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_function.h"
#include "ddk_system_allocator.h"
#include "ddk_lend_from_this.h"

namespace ddk
{
namespace detail
{

template<typename Key, typename Value>
class map_node
{
public:
    typedef Key key_type;
    typedef Value value_type;

    map_node() = default;
    template<typename ... TT>
    map_node(const Key& i_key,TT&& ... i_args);
    virtual ~map_node();
    inline unique_pointer_wrapper<map_node<Key,Value>> replace_node(unique_reference_wrapper<map_node<Key,Value>> other);
    template<typename Map>
    inline unique_reference_wrapper<map_node<Key,Value>> extract_node(lent_pointer_wrapper<Map> i_map = nullptr);
    inline lent_pointer_wrapper<map_node<Key,Value>> get_parent();
    inline void set_parent(lent_pointer_wrapper<map_node<Key,Value>> parent);
    inline void set_left_child(unique_pointer_wrapper<map_node<Key,Value>> i_left);
    inline lent_pointer_wrapper<map_node<Key,Value>> get_left_child();
    inline lent_pointer_wrapper<const map_node<Key,Value>> get_left_child() const;
    inline unique_pointer_wrapper<map_node<Key,Value>> extract_left_child();
    inline void set_right_child(unique_pointer_wrapper<map_node<Key,Value>> i_right);
    inline lent_pointer_wrapper<const map_node<Key,Value>> get_right_child() const;
    inline lent_pointer_wrapper<map_node<Key,Value>> get_right_child();
    inline unique_pointer_wrapper<map_node<Key,Value>> extract_right_child();
    inline std::pair<const Key,Value>& get_value();
    inline const std::pair<const Key,Value>& get_value() const;

private:
    std::pair<const Key,Value> m_value;
    lent_pointer_wrapper<map_node<Key,Value>> m_parent = nullptr;
    unique_pointer_wrapper<map_node<Key,Value>> m_leftChild;
    unique_pointer_wrapper<map_node<Key,Value>> m_rightChild;
};

template<typename Key,typename Value>
using map_node_unique_ref = unique_reference_wrapper<map_node<Key,Value>>;
template<typename Key,typename Value>
using map_node_unique_ptr = unique_pointer_wrapper<map_node<Key,Value>>;
template<typename Key,typename Value>
using map_node_lent_ref = lent_reference_wrapper<map_node<Key,Value>>;
template<typename Key,typename Value>
using map_node_const_lent_ref = lent_reference_wrapper<const map_node<Key,Value>>;
template<typename Key,typename Value>
using map_node_lent_ptr = lent_pointer_wrapper<map_node<Key,Value>>;
template<typename Key,typename Value>
using map_node_const_lent_ptr = lent_pointer_wrapper<const map_node<Key,Value>>;

template<typename Map,typename Node>
class EmptyBalancer
{
public:
    typedef EmptyBalancer type;
    typedef typename Map::key_t key_t;
    typedef typename Map::value_t value_t;

    inline static map_node_unique_ref<key_t,value_t> balance(lent_reference_wrapper<Map> i_map, map_node_lent_ref<key_t,value_t> i_node)
    {
        return promote_to_ref(std::move(i_map->m_root));
    };
};

template<typename Key, typename Value, typename Node, template<typename> class Allocator,template<typename,typename> class Balancer>
class map_impl : protected lend_from_this<map_impl<Key,Value,Node,Allocator,Balancer>>
{
    static_assert(std::is_const<Value>::value == false && std::is_reference<Value>::value == false,"Const/reference types are not supported");

    friend class lend_from_this<map_impl<Key,Value,Node,Allocator,Balancer>>;
    typedef unique_reference_wrapper<Node> map_node_unique_ref;
    typedef unique_pointer_wrapper<Node> map_node_unique_ptr;
    typedef lent_reference_wrapper<Node> map_node_lent_ref;
    typedef lent_reference_wrapper<const Node> map_node_const_lent_ref;
    typedef lent_pointer_wrapper<Node> map_node_lent_ptr;
    typedef lent_pointer_wrapper<const Node> map_node_const_lent_ptr;
    typedef Balancer<map_impl<Key,Value,Node,Allocator,Balancer>,Node> _balancer_t;
    friend class Balancer<map_impl<Key,Value,Node,Allocator,Balancer>,Node>;
    friend class map_node<Key,Value>;

public:
    struct map_iterator
    {
        friend class map_impl<Key,Value,Node,Allocator,Balancer>;
    public:
        typedef std::pair<const Key,Value&> reference;
        typedef std::pair<const Key,const Value&> const_reference;
        typedef std::pair<const Key,Value>* pointer;
        typedef const std::pair<const Key,Value>* const_pointer;

        map_iterator() = default;
        map_iterator(lent_pointer_wrapper<map_impl> i_mapImpl, map_node_lent_ptr i_currNode);

        reference operator*();
        pointer operator->();
        const_pointer operator->() const;
        const_reference operator*() const;
        map_iterator operator++() const;
        map_iterator operator++(int) const;
        map_iterator operator--() const;
        map_iterator operator--(int) const;
        bool operator==(const map_iterator& other) const;
        bool operator!=(const map_iterator& other) const;

    private:
        mutable lent_pointer_wrapper<map_impl> m_mapImpl = nullptr;
        mutable map_node_lent_ptr m_currNode = nullptr;
    };
    struct const_map_iterator
    {
        friend class map_impl<Key,Value,Node,Allocator,Balancer>;
    public:
        typedef std::pair<const Key,Value&> reference;
        typedef std::pair<const Key,const Value&> const_reference;
        typedef std::pair<const Key,Value>* pointer;
        typedef const std::pair<const Key,Value>* const_pointer;

        const_map_iterator() = default;
        const_map_iterator(lent_pointer_wrapper<const map_impl> i_mapImpl,map_node_const_lent_ptr i_currNode);
        const_map_iterator(const map_iterator& other);

        const_reference operator*() const;
        const_pointer operator->() const;
        const_map_iterator operator++() const;
        const_map_iterator operator++(int) const;
        const_map_iterator operator--() const;
        const_map_iterator operator--(int) const;
        bool operator==(const const_map_iterator& other) const;
        bool operator!=(const const_map_iterator& other) const;

    private:
        mutable lent_pointer_wrapper<const map_impl> m_mapImpl = nullptr;
        mutable map_node_const_lent_ptr m_currNode = nullptr;
    };

    typedef Key key_t;
    typedef Value value_t;
    typedef Node map_node_t;
    typedef Balancer<map_impl<Key,Value,Node,Allocator,Balancer>,Node> balancer_t;
    typedef typename std::add_const<Key>::type const_key_t;
    typedef typename std::add_const<Value>::type const_value_t;
    typedef typename std::remove_reference<typename std::remove_const<Value>::type>::type value_type;
    typedef typename std::remove_reference<Value>::type& reference;
    typedef const typename std::remove_reference<Value>::type& const_reference;
    typedef typename std::remove_reference<Value>::type* pointer;
    typedef const typename std::remove_reference<Value>::type* const_pointer;
    typedef typename std::add_lvalue_reference<Key>::type key_reference;
    typedef typename std::add_lvalue_reference<typename std::add_const<Key>::type>::type const_key_reference;
    typedef Key key_type;
    typedef function<bool(const_key_reference,const_key_reference)> compare_func;
    typedef map_iterator iterator;
    typedef const_map_iterator const_iterator;
    template<typename MMap,typename NNode>
    using balancer = Balancer<MMap,NNode>;

    map_impl();
    map_impl(const compare_func& i_compare);
    map_impl(const_key_reference key, const_reference value, compare_func i_compare = nullptr);
    map_impl(const map_impl& other);
    map_impl(map_impl&& other);
    virtual ~map_impl();
    map_impl& operator=(const map_impl& other);
    map_impl& operator=(map_impl&& other);
    const_iterator find(const_key_reference key) const;
    iterator find(const_key_reference key);
    const_reference operator[](const_key_reference key) const;
    reference operator[](const_key_reference key);
    template<typename ... Args>
    std::pair<bool,iterator> emplace(const_key_reference i_key,Args&& ... i_args);
    iterator erase(const iterator& itMap);
    const_iterator erase(const const_iterator& itMap);
    iterator erase(const_key_reference i_key);
    void clear();
    iterator begin();
    iterator last();
    const_iterator begin() const;
    const_iterator last() const;
    iterator end();
    const_iterator end() const;
    size_t size() const;
    bool empty() const;

private:
    template<typename Arg>
    inline void assign(const_key_reference key, Arg&& i_arg);
    void erase_node(map_node_lent_ref i_node);
    map_node_lent_ref add_node(const std::pair<const Key,Value>& i_value);
    inline void _onNodeInserted(map_node_lent_ref node);
    inline void _onNodeErased(map_node_lent_ref node);
    inline map_node_lent_ptr search(const Key& key) const;
    inline void insert(map_node_unique_ref other, map_node_lent_ref i_rootNode) const;
    inline map_node_lent_ptr get_closest_smaller_node(const_key_reference i_key) const;
    inline map_node_lent_ptr get_closest_bigger_node(const_key_reference i_key) const;

protected:
    typedef Allocator<map_node_t> allocator_t;
    typedef std::pair<const Key,Value> pair_t;

    inline map_node_lent_ptr get_first_elem() const;
    inline map_node_lent_ptr get_last_elem() const;
    inline map_node_lent_ptr get_next_elem(map_node_lent_ref i_currNode) const;
    inline map_node_lent_ptr get_prev_elem(map_node_lent_ref i_currNode) const;
    inline void create_tree(const map_impl& tree);
    inline void clear_tree();
    inline map_node_unique_ref allocate(map_node_const_lent_ref i_node) const;
    template<typename ... Args>
    inline map_node_unique_ref allocate(const_key_reference key, Args&& ... i_args) const;
    inline map_node_lent_ref findMin(map_node_lent_ref node) const;
    inline map_node_lent_ref findMax(map_node_lent_ref node) const;

    allocator_t m_alloc;
    compare_func m_compare;
    map_node_unique_ptr m_root;
};

}

template<typename Key,typename Value,template<typename> class Allocator = typed_system_allocator,template<typename,typename> class Balancer = detail::EmptyBalancer>
using map = detail::map_impl<Key,Value,detail::map_node<Key,Value>,Allocator,Balancer>;

}

#include "ddk_map.inl"
