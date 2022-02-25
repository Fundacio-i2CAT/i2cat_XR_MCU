#pragma once

#include "ddk_map.h"
#include "ddk_system_allocator.h"
#include "ddk_multi_map_balancer.h"
#include "ddk_template_helper.h"
#include "ddk_template_helper.h"
#include "ddk_type_concepts.h"
#include "ddk_concepts.h"

namespace ddk
{
namespace detail
{

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
class multi_map_value : public Map<Key,multi_map_value<Key,Value,Map,Allocator,Balancer>,Allocator>
{
public:
    typedef multi_map_value<Key,Value,Map,Allocator,Balancer> node_value_t;
    typedef Map<Key,node_value_t,Allocator> map_t;
    typedef Value underlying_type;
    typedef typename map_t::map_node_t map_node_t;
    typedef typename std::add_lvalue_reference<Value>::type reference;
    typedef typename std::add_lvalue_reference<typename std::add_const<Value>::type>::type const_reference;
    typedef typename map_t::value_t value_t;
    typedef typename map_t::key_t key_t;
    typedef typename map_t::iterator iterator;
    typedef typename map_t::const_iterator const_iterator;

    using map_t::operator[];

    multi_map_value() = default;
    multi_map_value(const Value& i_value);
    multi_map_value(Value&& i_value);
    multi_map_value(const multi_map_value& other);
    multi_map_value(multi_map_value&& other);
    multi_map_value& operator=(const multi_map_value& other);
    multi_map_value& operator=(const Value& i_val);
    multi_map_value& operator=(multi_map_value&& other);
    multi_map_value& operator=(Value&& i_val);
    inline reference get_value();
    inline const_reference get_value() const;
    inline operator reference();
    inline operator const_reference() const;
    inline reference at();
    inline const_reference at() const;
    TEMPLATE(typename ... Keys)
    REQUIRES(IS_CONVERTIBLE(Keys,Key)...)
    inline reference at(const Key& i_key, Keys&& ... i_keys);
    TEMPLATE(typename ... Keys)
    REQUIRES(IS_CONVERTIBLE(Keys,Key)...)
    inline const_reference at(const Key& i_key, Keys&& ... i_keys) const;
    inline size_t size(const function<bool(const_reference)>& filter = nullptr) const;

protected:
    Value m_value;
};

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
class multi_map_impl : public Map<Key,Value,Allocator>
{
public:
    typedef Map<Key,Value,Allocator> map_t;
    typedef typename Value::underlying_type underlying_type;
    typedef typename map_t::map_node_t map_node_t;
    typedef typename map_t::reference reference;
    typedef typename map_t::const_reference const_reference;
    typedef typename map_t::pointer pointer;
    typedef typename map_t::const_pointer const_pointer;
    typedef typename map_t::iterator iterator;
    typedef typename map_t::const_iterator const_iterator;

    multi_map_impl() = default;
    multi_map_impl(const multi_map_impl&);
    multi_map_impl& operator=(const multi_map_impl&);
    TEMPLATE(typename ... Keys)
    REQUIRES(IS_CONVERTIBLE(Keys,Key)...)
    underlying_type& at(const Key& i_key, Keys&& ... i_keys);
    TEMPLATE(typename ... Keys)
    REQUIRES(IS_CONVERTIBLE(Keys,Key)...)
    const underlying_type& at(const Key& i_key, Keys&& ... i_keys) const;
};

}

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator = typed_system_allocator>
using multi_map = detail::multi_map_impl<Key,detail::multi_map_value<Key,Value,Map,Allocator,recursive_balancer::template Balancer>,Map,Allocator,multi_balancer::template Balancer>;

}

#include "ddk_multi_map.inl"
#include "ddk_multi_map_adaptor.h"