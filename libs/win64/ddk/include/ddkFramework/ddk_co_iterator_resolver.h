#pragma once

#include "ddk_iterable_adaptor_resolver.h"

namespace ddk
{
namespace detail
{

template<bool,typename,typename>
struct co_iterator_type_correspondence_resolver;

template<bool IsConst,typename Adaptor>
struct co_iterator_type_correspondence_resolver<IsConst,const_forward_iterable_type,Adaptor>
{
private:
    typedef typename mpl::static_if<IsConst,const typename Adaptor::value_type,typename Adaptor::value_type>::type value_type;

public:
	typedef co_forward_iterator<value_type> type;
	typedef const_input_action action;
};
template<bool IsConst,typename Adaptor>
struct co_iterator_type_correspondence_resolver<IsConst,forward_iterable_type,Adaptor>
{
private:
    typedef typename mpl::static_if<IsConst,const typename Adaptor::value_type,typename Adaptor::value_type>::type value_type;

public:
	typedef co_forward_iterator<value_type> type;
	typedef const_input_action action;
};
template<bool IsConst,typename Adaptor>
struct co_iterator_type_correspondence_resolver<IsConst,const_bidirectional_iterable_type,Adaptor>
{
private:
    typedef typename mpl::static_if<IsConst,const typename Adaptor::value_type,typename Adaptor::value_type>::type value_type;

public:
	typedef co_bidirectional_iterator<value_type> type;
	typedef const_bidirectional_action action;
};
template<bool IsConst,typename Adaptor>
struct co_iterator_type_correspondence_resolver<IsConst,bidirectional_iterable_type,Adaptor>
{
private:
    typedef typename mpl::static_if<IsConst,const typename Adaptor::value_type,typename Adaptor::value_type>::type value_type;

public:
	typedef co_bidirectional_iterator<value_type> type;
	typedef const_bidirectional_action action;
};
template<bool IsConst,typename Adaptor>
struct co_iterator_type_correspondence_resolver<IsConst,const_random_access_iterable_type,Adaptor>
{
private:
    typedef typename mpl::static_if<IsConst,const typename Adaptor::value_type,typename Adaptor::value_type>::type value_type;

public:
	typedef co_random_access_iterator<value_type> type;
	typedef const_random_access_action action;
};
template<bool IsConst,typename Adaptor>
struct co_iterator_type_correspondence_resolver<IsConst,random_access_iterable_type,Adaptor>
{
private:
    typedef typename mpl::static_if<IsConst,const typename Adaptor::value_type,typename Adaptor::value_type>::type value_type;

public:
	typedef co_random_access_iterator<value_type> type;
	typedef const_random_access_action action;
};

template<typename Iterable>
using co_iterator_correspondence = typename co_iterator_type_correspondence_resolver<std::is_const<Iterable>::value,iterable_adaptor_type_correspondence<typename std::remove_reference<Iterable>::type>,iterable_adaptor<typename std::remove_reference<Iterable>::type>>::type;

}
}
