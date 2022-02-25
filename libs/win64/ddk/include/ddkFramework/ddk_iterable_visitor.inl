
#include "ddk_function.h"
#include "ddk_iterable_action.h"
#include "ddk_fiber_utils.h"
#include "ddk_iterable_exceptions.h"
#include "ddk_iterable_adaptor_resolver.h"

namespace ddk
{
namespace detail
{

template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
template<typename Action, typename FFunction>
action_visitor_base<Iterable,FinalAction,Function,Adaptor>::action_visitor_base(Iterable& i_iterable, Action&& i_initialAction, FFunction&& i_sink, action_state_lent_ptr i_actionStatePtr)
: m_adaptor(i_iterable,i_initialAction)
, m_actionStatePtr(i_actionStatePtr)
, m_currAction(go_no_place)
, m_sink(std::forward<FFunction>(i_sink))
{
}
template<typename Iterable,typename FinalAction,typename Function,typename Adaptor>
bool action_visitor_base<Iterable,FinalAction,Function,Adaptor>::valid() const noexcept
{
	return m_adaptor.valid();
}
template<typename Iterable,typename FinalAction,typename Function,typename Adaptor>
template<typename Visitor>
void action_visitor_base<Iterable,FinalAction,Function,Adaptor>::loop()
{
	Visitor& thisVisitor = static_cast<Visitor&>(*this);

	while(m_currAction.visit(thisVisitor));
}
template<typename Iterable,typename FinalAction,typename Function,typename Adaptor>
template<typename T>
void action_visitor_base<Iterable,FinalAction,Function,Adaptor>::apply(T&& i_value) const
{
	m_currAction = eval(m_sink,std::forward<T>(i_value));
}

template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,const_input_action,Adaptor>::operator()(const go_forward_action& i_action)
{
	const difference_type pendingShift = this->m_adaptor.forward_next_value_in(*this);

	if(pendingShift == 0)
	{
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(go_to_place(1,1));
		}

		return true;
	}
	else
	{
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(action_error(ActionError::ShiftError,ShiftActionError(pendingShift)));
		}

		return false;
	}
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,input_action,Adaptor>::operator()(const stop_action& i_action)
{
	if(this->m_actionStatePtr)
	{
		this->m_actionStatePtr->forward_result(i_action);
	}

	return false;
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,input_action,Adaptor>::operator()(const erase_action& i_action)
{
    if constexpr (std::is_const<Iterable>::value == false)
    {
		if(this->m_adaptor.forward_erase_value_in(*this))
		{
			if(this->m_actionStatePtr)
			{
				this->m_actionStatePtr->forward_result(i_action);
			}
		}
		else
		{
			if(this->m_actionStatePtr)
			{
				this->m_actionStatePtr->forward_result(action_error(ActionError::RemovalError,EraseActionError::NonExistingValue));
			}
		}
	}
    else
    {
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(action_error(ActionError::RemovalError,EraseActionError::ErasingFromConstantIterable));
		}
    }

	return true;
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,input_action,Adaptor>::operator()(const add_action& i_action)
{
    if constexpr (std::is_const<Iterable>::value == false)
    {
		typedef typename Iterable::value_type value_type;

		if(this->m_adaptor.forward_add_value_in(i_action.template get<value_type>(),*this))
		{
			if(this->m_actionStatePtr)
			{
				this->m_actionStatePtr->forward_result(i_action);
			}
		}
		else
		{
			if(this->m_actionStatePtr)
			{
				this->m_actionStatePtr->forward_result(action_error(ActionError::AdditionError,AddActionError::NonConvertibleType));
			}
		}
    }
    else
    {
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(action_error(ActionError::AdditionError,AddActionError::AddingToConstantIterable));
		}
    }

	return true;
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,const_bidirectional_action,Adaptor>::operator()(const go_backward_action& i_action)
{
	const difference_type pendingShift = this->m_adaptor.forward_prev_value_in(*this);

	if(pendingShift == 0)
	{
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(go_to_place(-1,-1));
		}

		return true;
	}
	else
	{
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(action_error(ActionError::ShiftError,ShiftActionError(pendingShift)));
		}

		return false;
	}
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,bidirectional_action,Adaptor>::operator()(const go_backward_action& i_action)
{
	const difference_type pendingShift = this->m_adaptor.forward_prev_value_in(*this);

	if(pendingShift == 0)
	{
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(go_to_place(-1,-1));
		}

		return true;
	}
	else
	{
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(action_error(ActionError::ShiftError,ShiftActionError(pendingShift)));
		}

		return false;
	}
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,const_random_access_action,Adaptor>::operator()(const shift_action& i_action)
{
	const difference_type currShift = i_action.shifting();
	const difference_type pendingShift = this->m_adaptor.forward_shift_value_in(currShift,*this);

	if(pendingShift == 0)
	{
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(go_to_place(currShift,currShift));
		}

		return true;
	}
	else
	{
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(action_error(ActionError::ShiftError,ShiftActionError(pendingShift)));
		}

		return false;
	}
}
template<typename Iterable,typename FinalAction, typename Function,typename Adaptor>
bool action_visitor<Iterable,FinalAction,Function,random_access_action,Adaptor>::operator()(const shift_action& i_action)
{
	const difference_type currShift = i_action.shifting();
	const difference_type pendingShift = this->m_adaptor.forward_shift_value_in(currShift,*this);

	if(pendingShift == 0)
	{
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(go_to_place(currShift,currShift));
		}

		return true;
	}
	else
	{
		if(this->m_actionStatePtr)
		{
			this->m_actionStatePtr->forward_result(action_error(ActionError::ShiftError,ShiftActionError(pendingShift)));
		}

		return false;
	}
}

}

template<typename Iterable, typename Function, typename Action>
void visit_iterator(Iterable& i_iterable, Function&& i_sink, const Action& i_initialAction, action_state_lent_ptr i_actionStatePtr)
{
	typedef detail::action_visitor<Iterable,Action,Function,Action,iterable_adaptor<Iterable>> action_visitor_t;
	action_visitor_t actionVisitor(i_iterable,i_initialAction.template get_as<shift_action>(),std::forward<Function>(i_sink),i_actionStatePtr);

	if(actionVisitor.valid())
	{
		actionVisitor.template loop<action_visitor_t>();
	}

	suspend();
}

}
