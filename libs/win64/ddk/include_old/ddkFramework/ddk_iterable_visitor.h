#pragma once

#include "ddk_iterable_state.h"
#include "ddk_iterable_action.h"
#include "ddk_scoped_enum.h"

namespace ddk
{
namespace detail
{

template<typename Iterable,typename FinalAction, typename Function, typename Adaptor>
struct action_visitor_base : public static_visitor<bool>
{
public:
	template<typename Action, typename FFunction>
	action_visitor_base(Iterable& i_iterable, Action&& i_initialAction, FFunction&& i_sink, action_state_lent_ptr i_actionStatePtr);

	template<typename Visitor>
	inline void loop();
	template<typename T>
	inline void apply(T&& i_values) const;
	inline bool valid() const noexcept;

protected:
	typedef typename Adaptor::difference_type difference_type;

	Adaptor m_adaptor;
	action_state_lent_ptr m_actionStatePtr;
	mutable FinalAction m_currAction;
	Function m_sink;
};

template<typename Iterable, typename FinalAction,typename Function, typename Action, typename Adaptor>
struct action_visitor;

template<typename Iterable, typename FinalAction, typename Function,typename Adaptor>
struct action_visitor<Iterable,FinalAction,Function,const_input_action,Adaptor>: public action_visitor_base<Iterable,FinalAction,Function,Adaptor>
{
public:
	using action_visitor_base<Iterable,FinalAction,Function,Adaptor>::action_visitor_base;
	using typename action_visitor_base<Iterable,FinalAction,Function,Adaptor>::difference_type;
	typedef const_input_action action;

	inline bool operator()(const go_forward_action&);
};

template<typename Iterable, typename FinalAction, typename Function,typename Adaptor>
struct action_visitor<Iterable,FinalAction,Function,input_action,Adaptor> : public action_visitor<Iterable,FinalAction,Function,const_input_action,Adaptor>
{
public:
    using action_visitor<Iterable,FinalAction,Function,const_input_action,Adaptor>::action_visitor;
	using action_visitor<Iterable,FinalAction,Function,const_input_action,Adaptor>::operator();
	using typename action_visitor<Iterable,FinalAction,Function,const_input_action,Adaptor>::difference_type;
	typedef input_action action;

	inline bool operator()(const stop_action&);
	inline bool operator()(const erase_action&);
	inline bool operator()(const add_action&);
};

template<typename Iterable, typename FinalAction, typename Function,typename Adaptor>
struct action_visitor<Iterable,FinalAction,Function,const_bidirectional_action,Adaptor> : public action_visitor<Iterable,FinalAction,Function,const_input_action,Adaptor>
{
public:
    using action_visitor<Iterable,FinalAction,Function,const_input_action,Adaptor>::action_visitor;
	using action_visitor<Iterable,FinalAction,Function,const_input_action,Adaptor>::operator();
	using typename action_visitor<Iterable,FinalAction,Function,const_input_action,Adaptor>::difference_type;
	typedef const_bidirectional_action action;

	inline bool operator()(const go_backward_action&);
};

template<typename Iterable, typename FinalAction, typename Function,typename Adaptor>
struct action_visitor<Iterable,FinalAction,Function,bidirectional_action,Adaptor>: public action_visitor<Iterable,FinalAction,Function,input_action,Adaptor>
{
public:
	using action_visitor<Iterable,FinalAction,Function,input_action,Adaptor>::action_visitor;
	using action_visitor<Iterable,FinalAction,Function,input_action,Adaptor>::operator();
	using typename action_visitor<Iterable,FinalAction,Function,input_action,Adaptor>::difference_type;
	typedef bidirectional_action action;

	inline bool operator()(const go_backward_action&);
};

template<typename Iterable, typename FinalAction, typename Function,typename Adaptor>
struct action_visitor<Iterable,FinalAction,Function,const_random_access_action,Adaptor> : public action_visitor<Iterable,FinalAction,Function,const_bidirectional_action,Adaptor>
{
public:
    using action_visitor<Iterable,FinalAction,Function,const_bidirectional_action,Adaptor>::action_visitor;
	using action_visitor<Iterable,FinalAction,Function,const_bidirectional_action,Adaptor>::operator();
	using typename action_visitor<Iterable,FinalAction,Function,const_bidirectional_action,Adaptor>::difference_type;
	typedef const_random_access_action action;

	inline bool operator()(const shift_action&);
};

template<typename Iterable, typename FinalAction, typename Function,typename Adaptor>
struct action_visitor<Iterable,FinalAction,Function,random_access_action,Adaptor>: public action_visitor<Iterable,FinalAction,Function,bidirectional_action,Adaptor>
{
public:
	using action_visitor<Iterable,FinalAction,Function,bidirectional_action,Adaptor>::action_visitor;
	using action_visitor<Iterable,FinalAction,Function,bidirectional_action,Adaptor>::operator();
	using typename action_visitor<Iterable,FinalAction,Function,bidirectional_action,Adaptor>::difference_type;
	typedef const_random_access_action action;

	inline bool operator()(const shift_action&);
};

}

template<typename Iterable, typename Function,typename Action>
inline void visit_iterator(Iterable& i_iterable, Function&& i_sink, const Action& i_initialAction, action_state_lent_ptr i_actionStatePtr = nullptr);

}

#include "ddk_iterable_visitor.inl"
