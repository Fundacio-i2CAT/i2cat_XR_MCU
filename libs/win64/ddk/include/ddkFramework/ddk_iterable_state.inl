
#include "ddk_iterable_state_visitor.h"

namespace ddk
{

template<typename Action>
void iterable_state::apply(const Action& i_action)
{
    m_currPos += i_action.shifted();
}
template<typename ... Types>
void iterable_state::apply(const variant<Types...>& i_action)
{
	const action_shift_visitor actionVisitor(m_currPos);

	m_currPos = i_action.visit(actionVisitor);
}

}
