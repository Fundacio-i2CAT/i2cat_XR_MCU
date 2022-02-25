
#include "ddk_await.h"
#include "ddk_transformed_iterable_action_impl.h"
#include "ddk_iterable_interface_utils.h"

namespace ddk
{
namespace detail
{

template<typename Traits>
iterable<Traits>::iterable(iterable_impl_dist_ref<iterable_base_traits> i_iterableImpl)
: m_iterableImpl(std::move(i_iterableImpl))
{
}
template<typename Traits>
iterable<Traits>::iterable(const iterable& other)
: m_iterableImpl(other.m_iterableImpl)
, m_iterableState(other.m_iterableState)
{
}
template<typename Traits>
template<typename TTraits>
iterable<Traits>::iterable(const iterable<TTraits>& other)
: m_iterableImpl(make_iterable_impl<transformed_iterable_action_impl<Traits,TTraits>>(other.m_iterableImpl,[](const typename TTraits::action& i_action){ return action(i_action); }))
{
}
template<typename Traits>
iterable<Traits>::~iterable()
{
}
template<typename Traits>
TEMPLATE(typename Function)
REQUIRED(IS_CALLABLE(Function))
action_result iterable<Traits>::iterate(Function&& i_try, const shift_action& i_initialAction)
{
	try
	{
		m_iterableImpl->iterate_impl(make_function([&](reference i_value)
		{
			typedef typename mpl::make_sequence<0,mpl::aqcuire_callable_args_type<Function>::type::size()>::type range_seq;

			m_iterableState.apply(m_currAction);

			call_iterable_payload(range_seq{},std::forward<Function>(i_try),i_value);

			return m_currAction;
		}),i_initialAction,lend(m_actionState));
	}
	catch(const suspend_exception&)
	{
	}

	return m_actionState->get();
}
template<typename Traits>
TEMPLATE(typename Function)
REQUIRED(IS_CALLABLE(Function))
action_result iterable<Traits>::iterate(Function&& i_try, const shift_action& i_initialAction) const
{
	try
	{
		m_iterableImpl->iterate_impl(make_function([&](const_reference i_value)
		{
			typedef typename mpl::make_sequence<0,mpl::aqcuire_callable_args_type<Function>::type::size()>::type range_seq;

			m_iterableState.apply(m_currAction);

			call_iterable_payload(range_seq{},std::forward<Function>(i_try),i_value);

			return m_currAction;
		}),i_initialAction,lend(m_actionState));
	}
	catch(const suspend_exception&)
	{
	}

	return m_actionState->get();
}
template<typename Traits>
TEMPLATE(typename Function)
REQUIRED(IS_CALLABLE(Function))
action_result iterable<Traits>::co_iterate(Function&& i_try, const shift_action& i_initialAction)
{
	m_executor = detail::await_executor<void>(make_function(m_iterableImpl.get(),&iterable_impl_interface<iterable_base_traits>::iterate_impl,make_function(this,&iterable<Traits>::private_iterate),i_initialAction,lend(m_actionState)));

    while(true)
    {
        if(m_executor.resume())
        {
			m_currAction = iterable_base_traits::default_action();

			try
            {
                m_iterableState.apply(m_currAction);

                eval(std::forward<Function>(i_try),make_iterable_value<iterable_value>(*m_iterableValueContainer.template extract<reference>(),make_function(this,&iterable<Traits>::resolve_action),static_cast<iterable_interface&>(*this)));
            }
            catch(const suspend_exception&)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

	return m_actionState->get();
}
template<typename Traits>
TEMPLATE(typename Function)
REQUIRED(IS_CALLABLE(Function))
action_result iterable<Traits>::co_iterate(Function&& i_try, const shift_action& i_initialAction) const
{
    typedef action(iterable<Traits>::*func_ptr)(const_reference)const;

	m_executor = detail::await_executor<void>(make_function(m_iterableImpl.get(),&iterable_impl_interface<iterable_base_traits>::iterate_impl,make_function(this,&iterable<Traits>::private_iterate),i_initialAction,lend(m_actionState)));

    while(true)
    {
        if(m_executor.resume())
        {
			m_currAction = iterable_base_traits::default_action();

			try
            {
                m_iterableState.apply(m_currAction);

                eval(std::forward<Function>(i_try),make_iterable_value<iterable_const_value>(*m_iterableValueContainer.template extract<const_reference>(),make_function(this,&iterable<Traits>::resolve_action),const_cast<iterable_interface&>(static_cast<const iterable_interface&>(*this))));
            }
            catch(const suspend_exception&)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

	return m_actionState->get();
}
template<typename Traits>
bool iterable<Traits>::operator==(const std::nullptr_t&) const
{
    return m_iterableImpl == nullptr;
}
template<typename Traits>
bool iterable<Traits>::operator!=(const std::nullptr_t&) const
{
    return m_iterableImpl != nullptr;
}
template<typename Traits>
size_t iterable<Traits>::size() const
{
    return m_iterableImpl->size();
}
template<typename Traits>
bool iterable<Traits>::empty() const
{
    return m_iterableImpl->empty();
}
template<typename Traits>
iterable_state& iterable<Traits>::get_state()
{
    return m_iterableState;
}
template<typename Traits>
const iterable_state& iterable<Traits>::get_state() const
{
    return m_iterableState;
}
template<typename Traits>
action_state_lent_ref iterable<Traits>::get_action_state()
{
    return lend(m_actionState);
}
template<typename Traits>
action_state_const_lent_ref iterable<Traits>::get_action_state() const
{
    return lend(m_actionState);
}
template<typename Traits>
typename iterable<Traits>::action iterable<Traits>::private_iterate(reference i_value)
{
	action_result actionResult = m_actionState->get();

	if(actionResult == success)
	{
		if(actionResult.is_base_of<shift_action>())
		{
			//in case of shift action consolidate it against result
			const shift_action& currReturnedAction = actionResult.get_as<shift_action>();
			const consolidate_visitor consolidateVisitor(currReturnedAction);

			if(currReturnedAction.step_by_step() == false || m_currAction.visit(consolidateVisitor))
			{
				//we already destroyed arena by extraction
				m_iterableValueContainer.template construct<const_reference>(i_value);

				m_executor.yield();
			}
		}
		else
		{
			//we already destroyed arena by extraction
			m_iterableValueContainer.template construct<const_reference>(i_value);

			m_executor.yield();
		}
	}

	return m_currAction;
}
template<typename Traits>
typename iterable<Traits>::action iterable<Traits>::private_iterate(const_reference i_value) const
{
	const action_result actionResult = m_actionState->get();

	if(actionResult == success)
	{
		if(actionResult.is_base_of<shift_action>())
		{
			//in case of shift action consolidate it against result
			const shift_action& currReturnedAction = actionResult.get_as<shift_action>();
			const consolidate_visitor consolidateVisitor(currReturnedAction);

			if(currReturnedAction.step_by_step() == false || m_currAction.visit(consolidateVisitor))
			{
				//we already destroyed arena by extraction
				m_iterableValueContainer.template construct<const_reference>(i_value);

				m_executor.yield();
			}
		}
		else
		{
			//we already destroyed arena by extraction
			m_iterableValueContainer.template construct<const_reference>(i_value);

			m_executor.yield();
		}
	}

	return m_currAction;
}
template<typename Traits>
typename iterable<Traits>::reference iterable<Traits>::resolve_action(const action& i_action)
{
    m_currAction = i_action;

    if (m_executor.resume())
    {
        m_iterableState.apply(m_currAction);

        return m_iterableValueContainer.template extract<reference>();
    }
    else
    {
        throw suspend_exception{get_current_fiber_id()};
    }
}
template<typename Traits>
typename iterable<Traits>::const_reference iterable<Traits>::resolve_action(const action& i_action) const
{
    m_currAction = i_action;

    if (m_executor.resume())
    {
        m_iterableState.apply(m_currAction);

        return m_iterableValueContainer.template extract<const_reference>();
    }
    else
    {
        throw suspend_exception{get_current_fiber_id()};
    }
}
template<typename Traits>
bool iterable<Traits>::forward_action(action i_action) const
{
	m_currAction = i_action;

	return true;
}

}
}
