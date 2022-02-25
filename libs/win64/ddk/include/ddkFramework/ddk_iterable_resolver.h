#pragma once

#include "ddk_iterable_adaptor_resolver.h"

namespace ddk
{
namespace detail
{

template<bool,typename,typename>
struct iterable_correspondence_resolver;

template<bool IsConst,typename Adaptor>
struct iterable_correspondence_resolver<IsConst,forward_iterable_type,Adaptor>
{
private:
    typedef typename Adaptor::value_type value_type;
    typedef typename mpl::static_if<IsConst,typename std::add_const<value_type>::type,value_type>::type qualified_value_type;

public:
	typedef forward_iterable<qualified_value_type> type;
};
template<bool IsConst,typename Adaptor>
struct iterable_correspondence_resolver<IsConst,const_forward_iterable_type,Adaptor>
{
private:
    typedef typename Adaptor::value_type value_type;
    typedef typename mpl::static_if<IsConst,typename std::add_const<value_type>::type,value_type>::type qualified_value_type;

public:
	typedef const_forward_iterable<qualified_value_type> type;
};
template<bool IsConst,typename Adaptor>
struct iterable_correspondence_resolver<IsConst,bidirectional_iterable_type,Adaptor>
{
private:
    typedef typename Adaptor::value_type value_type;
    typedef typename mpl::static_if<IsConst,typename std::add_const<value_type>::type,value_type>::type qualified_value_type;

public:
	typedef bidirectional_iterable<qualified_value_type> type;
};
template<bool IsConst,typename Adaptor>
struct iterable_correspondence_resolver<IsConst,const_bidirectional_iterable_type,Adaptor>
{
private:
    typedef typename Adaptor::value_type value_type;
    typedef typename mpl::static_if<IsConst,typename std::add_const<value_type>::type,value_type>::type qualified_value_type;

public:
	typedef const_bidirectional_iterable<qualified_value_type> type;
};
template<bool IsConst,typename Adaptor>
struct iterable_correspondence_resolver<IsConst,random_access_iterable_type,Adaptor>
{
private:
    typedef typename Adaptor::value_type value_type;
    typedef typename mpl::static_if<IsConst,typename std::add_const<value_type>::type,value_type>::type qualified_value_type;

public:
	typedef random_access_iterable<qualified_value_type> type;
};
template<bool IsConst,typename Adaptor>
struct iterable_correspondence_resolver<IsConst,const_random_access_iterable_type,Adaptor>
{
private:
    typedef typename Adaptor::value_type value_type;
    typedef typename mpl::static_if<IsConst,typename std::add_const<value_type>::type,value_type>::type qualified_value_type;

public:
	typedef const_random_access_iterable<qualified_value_type> type;
};

template<typename>
struct _iterable_correspondence;
template<typename Traits>
struct _iterable_correspondence<iterable<Traits>>
{
    typedef iterable<Traits> type;
};
template<typename Traits>
struct _iterable_correspondence<const iterable<Traits>>
{
    typedef iterable<Traits> type;
};
template<typename Iterable>
struct _iterable_correspondence
{
        typedef typename iterable_correspondence_resolver<std::is_const<Iterable>::value,iterable_adaptor_type_correspondence<mpl::remove_qualifiers<Iterable>>,iterable_adaptor<mpl::remove_qualifiers<Iterable>>>::type type;
};

template<typename Iterable>
using iterable_correspondence = typename _iterable_correspondence<Iterable>::type;

template<bool,typename,typename>
struct iterable_action_correspondence_resolver;

template<bool IsConst,typename Adaptor>
struct iterable_action_correspondence_resolver<IsConst,forward_iterable_type,Adaptor>
{
	typedef typename mpl::static_if<IsConst,const_forward_action,forward_action>::type type;
};
template<bool IsConst,typename Adaptor>
struct iterable_action_correspondence_resolver<IsConst,const_forward_iterable_type,Adaptor>
{
	typedef typename mpl::static_if<IsConst,const_forward_action,forward_action>::type type;
};
template<bool IsConst,typename Adaptor>
struct iterable_action_correspondence_resolver<IsConst,bidirectional_iterable_type,Adaptor>
{
	typedef typename mpl::static_if<IsConst,const_bidirectional_action,bidirectional_action>::type type;
};
template<bool IsConst,typename Adaptor>
struct iterable_action_correspondence_resolver<IsConst,const_bidirectional_iterable_type,Adaptor>
{
	typedef typename mpl::static_if<IsConst,const_bidirectional_action,bidirectional_action>::type type;
};
template<bool IsConst,typename Adaptor>
struct iterable_action_correspondence_resolver<IsConst,random_access_iterable_type,Adaptor>
{
	typedef typename mpl::static_if<IsConst,const_random_access_action,random_access_action>::type type;
};
template<bool IsConst,typename Adaptor>
struct iterable_action_correspondence_resolver<IsConst,const_random_access_iterable_type,Adaptor>
{
	typedef typename mpl::static_if<IsConst,const_random_access_action,random_access_action>::type type;
};

template<typename>
struct _iterable_action_correspondence;
template<typename Traits>
struct _iterable_action_correspondence<iterable<Traits>>
{
    typedef typename iterable<Traits>::action type;
};
template<typename Iterable>
struct _iterable_action_correspondence
{
        typedef typename iterable_action_correspondence_resolver<std::is_const<Iterable>::value,iterable_adaptor_type_correspondence<mpl::remove_qualifiers<Iterable>>,iterable_adaptor<mpl::remove_qualifiers<Iterable>>>::type type;
};

template<typename Iterable>
using iterable_action_correspondence = typename iterable_action_correspondence_resolver<std::is_const<Iterable>::value,iterable_adaptor_type_correspondence<mpl::remove_qualifiers<Iterable>>,iterable_adaptor<mpl::remove_qualifiers<Iterable>>>::type;

}
}
