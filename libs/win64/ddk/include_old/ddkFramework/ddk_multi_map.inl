
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace detail
{

template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::multi_map_value(const Value& i_value)
: map_t()
, m_value(i_value)
{
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::multi_map_value(Value&& i_value)
: map_t()
, m_value(std::move(i_value))
{
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::multi_map_value(const multi_map_value& other)
: map_t(other)
, m_value(other.m_value)
{
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::multi_map_value(multi_map_value&& other)
: map_t(std::move(other))
, m_value(std::move(other.m_value))
{
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>& multi_map_value<Key,Value,Map,Allocator,Balancer>::operator=(const multi_map_value& other)
{
    m_value = other.m_value;

    map_t::operator=(other);

    return *this;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>& multi_map_value<Key,Value,Map,Allocator,Balancer>::operator=(const Value& i_val)
{
    m_value = i_val;

    return *this;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>& multi_map_value<Key,Value,Map,Allocator,Balancer>::operator=(multi_map_value&& other)
{
    m_value = std::move(other.m_value);

    map_t::operator=(std::move(other));

    return *this;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>& multi_map_value<Key,Value,Map,Allocator,Balancer>::operator=(Value&& i_val)
{
    m_value = std::move(i_val);

    return *this;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::reference multi_map_value<Key,Value,Map,Allocator,Balancer>::get_value()
{
    return m_value;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::const_reference multi_map_value<Key,Value,Map,Allocator,Balancer>::get_value() const
{
    return m_value;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::operator reference()
{
    return m_value;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
multi_map_value<Key,Value,Map,Allocator,Balancer>::operator const_reference() const
{
    return m_value;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
size_t multi_map_value<Key,Value,Map,Allocator,Balancer>::size(const function<bool(const_reference)>& filter) const
{
    size_t res = 0;

    const_iterator itPoliThis = this->begin();

    for(; itPoliThis != this->end(); itPoliThis++)
    {
        if(filter == nullptr || filter(*itPoliThis))
        {
            res++;
        }

        res += itPoliThis->second.size();
    }

    return res;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::reference multi_map_value<Key,Value,Map,Allocator,Balancer>::at()
{
    return m_value;
}
template<typename Key, typename Value, template<typename,typename,template<typename>class> class Map,template<typename> class Allocator, template<typename,typename> class Balancer>
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::const_reference multi_map_value<Key,Value,Map,Allocator,Balancer>::at() const
{
    return m_value;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
TEMPLATE(typename ... Keys)
REQUIRED(IS_CONVERTIBLE(Keys,Key)...)
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::reference multi_map_value<Key,Value,Map,Allocator,Balancer>::at(const Key& i_key,Keys&& ... i_keys)
{
    multi_map_value<Key,Value,Map,Allocator,Balancer>& foundValue = map_t::operator[](i_key);

    return foundValue.at(std::forward<Keys>(i_keys)...);
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
TEMPLATE(typename ... Keys)
REQUIRED(IS_CONVERTIBLE(Keys,Key)...)
typename multi_map_value<Key,Value,Map,Allocator,Balancer>::const_reference multi_map_value<Key,Value,Map,Allocator,Balancer>::at(const Key& i_key,Keys&& ... i_keys) const
{
    const multi_map_value<Key,Value,Map,Allocator,Balancer>& foundValue = map_t::operator[](i_key);

    return foundValue.at(std::forward<Keys>(i_keys)...);
}

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
multi_map_impl<Key,Value,Map,Allocator,Balancer>::multi_map_impl(const multi_map_impl& other)
: map_t(other)
{
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
multi_map_impl<Key,Value,Map,Allocator,Balancer>& multi_map_impl<Key,Value,Map,Allocator,Balancer>::operator=(const multi_map_impl& other)
{
    map_t::operator=(other);

    return *this;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
TEMPLATE(typename ... Keys)
REQUIRED(IS_CONVERTIBLE(Keys,Key)...)
typename multi_map_impl<Key,Value,Map,Allocator,Balancer>::underlying_type& multi_map_impl<Key,Value,Map,Allocator,Balancer>::at(const Key& i_key, Keys&& ... i_keys)
{
    Value& foundValue = map_t::operator[](i_key);

    return foundValue.at(std::forward<Keys>(i_keys)...);
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator,template<typename,typename> class Balancer>
TEMPLATE(typename ... Keys)
REQUIRED(IS_CONVERTIBLE(Keys,Key)...)
const typename multi_map_impl<Key,Value,Map,Allocator,Balancer>::underlying_type& multi_map_impl<Key,Value,Map,Allocator,Balancer>::at(const Key& i_key, Keys&& ... i_keys) const
{
    const Value& foundValue = map_t::operator[](i_key);

    return foundValue.at(std::forward<Keys>(i_keys)...);
}

}
}