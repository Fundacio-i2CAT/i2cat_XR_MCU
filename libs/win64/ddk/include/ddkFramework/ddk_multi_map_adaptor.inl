
namespace ddk
{

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::iterable_adaptor(multi_map<Key,Value,Map,Allocator>& i_iterable,const ddk::shift_action& i_initialAction)
: m_iterable(i_iterable)
, m_nextMov(i_initialAction.target_shift())
, m_awaitable(await(make_function(this,&iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::navigate,i_initialAction)))
{
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
template<typename Sink>
typename iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::difference_type iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::forward_next_value_in(Sink&& i_sink)
{
    m_nextMov = 1;

    if(awaited_result<reference> res = resume(m_awaitable))
    {
        i_sink.apply(res.get());

        return 0;
    }
    else
    {
        m_valid = false;

        return 1;
    }
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
template<typename Sink>
typename iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::difference_type iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::forward_next_value_in(Sink&& i_sink) const
{
    if(awaited_result<reference> res = resume(m_awaitable))
    {
        i_sink.apply(res.get());

        return 0;
    }
    else
    {
        m_valid = false;

        return 1;
    }
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
template<typename Sink>
typename iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::difference_type iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::forward_prev_value_in(Sink&& i_sink)
{
    m_nextMov = -1;

    if(awaited_result<reference> res = resume(m_awaitable))
    {
        i_sink.apply(res.get());

        return 0;
    }
    else
    {
        m_valid = false;

        return -1;
    }
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
template<typename Sink>
typename iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::difference_type iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::forward_prev_value_in(Sink&& i_sink) const
{
    m_nextMov = -1;

    if(awaited_result<reference> res = resume(m_awaitable))
    {
        i_sink.apply(res.get());

        return 0;
    }
    else
    {
        m_valid = false;

        return -1;
    }
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::valid() const noexcept
{
    return m_valid;
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
void iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::_navigate(value_t& i_map, std::vector<Key>& i_preffix)
{
    typename value_t::iterator itCurr = (m_nextMov == 1) ? i_map.begin() : i_map.last();
    typename value_t::iterator itEnd = i_map.end();

    for(; itCurr != itEnd;)
    {
        value_t& nestedMap = itCurr->second;

        i_preffix.push_back(itCurr->first);

        value_type currValue = { i_preffix,itCurr->second };

        switch(m_nextMov)
        {
            case 1:
            {
                yield(currValue);

                if(nestedMap.empty() == false)
                {
                    _navigate(nestedMap,i_preffix);
                }

                ++itCurr;

                break;
            }
            case -1:
            {
                if(nestedMap.empty() == false)
                {
                    _navigate(nestedMap,i_preffix);
                }

                yield(currValue);

                --itCurr;

                break;
            }
            default:
            {
                break;
            }
        }

        i_preffix.erase(i_preffix.begin() + i_preffix.size() - 1);
    }
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
typename iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::reference iterable_adaptor<multi_map<Key,Value,Map,Allocator>>::navigate(const ddk::shift_action& i_initialAction)
{
    typename value_t::iterator itCurr = (i_initialAction.target_shift() > 0) ? m_iterable.begin() : m_iterable.last();
    typename value_t::iterator itEnd = m_iterable.end();
    std::vector<Key> preffix;

    m_nextMov = i_initialAction.target_shift();

    for(; itCurr != itEnd;)
    {
        value_t& nestedMap = itCurr->second;
        preffix.push_back(itCurr->first);
        value_type currValue = { preffix,itCurr->second };

        switch(m_nextMov)
        {
            case 1:
            {
                yield(currValue);

                if(nestedMap.empty() == false)
                {
                    _navigate(nestedMap,preffix);
                }

                ++itCurr;

                break;
            }
            case -1:
            {
                if(nestedMap.empty() == false)
                {
                    _navigate(nestedMap,preffix);
                }

                yield(currValue);

                --itCurr;

                break;
            }
            default:
            {
                break;
            }
        }

        preffix.erase(preffix.begin() + preffix.size() - 1);
    }

    suspend();

    return crash_on_return<reference>::value();
}

template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::iterable_adaptor(const multi_map<Key,Value,Map,Allocator>& i_iterable,const ddk::shift_action& i_initialAction)
: m_iterable(i_iterable)
, m_nextMov(i_initialAction.target_shift())
, m_awaitable(await(make_function(this,&iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::navigate,i_initialAction)))
{
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
template<typename Sink>
typename iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::difference_type iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::forward_next_value_in(Sink&& i_sink)
{
    m_nextMov = 1;

    if(awaited_result<const_reference> res = resume(m_awaitable))
    {
        i_sink.apply(res.get());

        return 0;
    }
    else
    {
        m_valid = false;

        return 1;
    }
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
template<typename Sink>
typename iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::difference_type iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::forward_next_value_in(Sink&& i_sink) const
{
    m_nextMov = 1;

    if(awaited_result<const_reference> res = resume(m_awaitable))
    {
        i_sink.apply(res.get());

        return 0;
    }
    else
    {
        m_valid = false;

        return 1;
    }
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
template<typename Sink>
typename iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::difference_type iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::forward_prev_value_in(Sink&& i_sink)
{
    m_nextMov = -1;

    if(awaited_result<const_reference> res = resume(m_awaitable))
    {
        i_sink.apply(res.get());

        return 0;
    }
    else
    {
        m_valid = false;

        return -1;
    }
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
template<typename Sink>
typename iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::difference_type iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::forward_prev_value_in(Sink&& i_sink) const
{
    m_nextMov = -1;

    if(awaited_result<const_reference> res = resume(m_awaitable))
    {
        i_sink.apply(res.get());

        return 0;
    }
    else
    {
        m_valid = false;

        return -1;
    }
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
void iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::_navigate(const value_t& i_map, std::vector<Key>& i_preffix)
{
    typename value_t::const_iterator itCurr = (m_nextMov == 1) ? i_map.begin() : i_map.last();
    typename value_t::const_iterator itEnd = i_map.end();

    for(; itCurr != itEnd;)
    {
        value_t& nestedMap = itCurr->second;

        i_preffix.push_back(itCurr->first);

        value_type currValue = { i_preffix,itCurr->second };

        switch(m_nextMov)
        {
            case 1:
            {
                yield(currValue);

                if(nestedMap.empty() == false)
                {
                    _navigate(nestedMap,i_preffix);
                }

                ++itCurr;

                break;
            }
            case -1:
            {
                if(nestedMap.empty() == false)
                {
                    _navigate(nestedMap,i_preffix);
                }

                yield(currValue);

                --itCurr;

                break;
            }
            default:
            {
                break;
            }
        }
    }
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
typename iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::const_reference iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::navigate(const ddk::shift_action& i_initialAction)
{
    typename value_t::const_iterator itCurr = (i_initialAction.target_shift() > 0) ? m_iterable.begin() : m_iterable.last();
    typename value_t::const_iterator itEnd = m_iterable.end();
    std::vector<Key> preffix;

    for(; itCurr != itEnd;)
    {
        value_t& nestedMap = itCurr->second;
        preffix.push_back(itCurr->first);
        value_type currValue = { preffix,itCurr->second };

        switch(m_nextMov)
        {
            case 1:
            {
                yield(currValue);

                if(nestedMap.empty() == false)
                {
                    _navigate(nestedMap,preffix);
                }

                ++itCurr;

                break;
            }
            case -1:
            {
                if(nestedMap.empty() == false)
                {
                    _navigate(nestedMap,preffix);
                }

                yield(currValue);

                --itCurr;

                break;
            }
            default:
            {
                break;
            }
        }
    }

    suspend();

    return crash_on_return<const_reference>::value();
}
template<typename Key,typename Value,template<typename,typename,template<typename>class> class Map,template<typename> class Allocator>
bool iterable_adaptor<const multi_map<Key,Value,Map,Allocator>>::valid() const noexcept
{
    return m_valid;
}

}
