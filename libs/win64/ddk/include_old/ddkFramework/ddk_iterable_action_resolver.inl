
#include "ddk_iterable_action.h"

namespace ddk
{
namespace detail
{

template<typename ... Types>
variant<Types...> reversable_action_resolver::resolve(const variant<Types...>& i_action) const
{
	typedef variant<Types...> action;

	if constexpr (variant<Types...>::template contains<go_forward_action>())
	{
		if (i_action.template is<go_forward_action>())
		{
			return (m_forward) ? action(go_next_place) : action(go_prev_place);
		}
	}
	if constexpr (variant<Types...>::template contains<go_backward_action>())
	{
		if (i_action.template is<go_backward_action>())
		{
			return (m_forward) ? action(go_prev_place) : action(go_next_place);
		}
	}
	if constexpr (variant<Types...>::template contains<shift_action>())
	{
		if (i_action.template is<shift_action>())
		{
			const shift_action& nestedAction = i_action.template get<shift_action>();

			return (m_forward) ? action(go_to_place(nestedAction.shifting())) : action(go_to_place(-nestedAction.shifting()));
		}
	}

	return i_action;
}

template<typename ... Types>
variant<Types...> alternate_action_resolver::resolve(const variant<Types...>& i_action) const
{
	typedef variant<Types...> action;

	if constexpr(variant<Types...>::template contains<shift_action>())
	{
		const int currIndex = m_currIndex;
		const bool forward = m_forward;

		if(i_action.template is<go_forward_action>())
		{
			m_currIndex++;
		}
		else if(i_action.template is<go_backward_action>())
		{
			m_currIndex--;
		}
		else if(i_action.template is<shift_action>())
		{
			const shift_action& nestedAction = i_action.template get<shift_action>();

			m_currIndex += nestedAction.shifting();
		}

		if(m_currIndex >= 0 && m_currIndex < m_constrain)
		{
			m_forward = !m_forward;

			return action(go_to_place(((forward) ? m_currIndex : -m_currIndex + m_constrain) - currIndex));
		}
		else
		{
			stop_iteration();
		}
	}
	else
	{
		static_assert(sizeof(action)==0,"Alternate orders demmand shiftable iterables.");
	}

	return i_action;
}

}
}
