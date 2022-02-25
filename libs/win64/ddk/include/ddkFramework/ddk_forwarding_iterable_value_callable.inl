
#include "ddk_iterable_action.h"

namespace ddk
{

template<typename Function,typename Action>
template<typename FFunction>
forwarding_iterable_value_callable<Function,Action>::forwarding_iterable_value_callable(FFunction&& i_function)
: m_function(std::forward<FFunction>(i_function))
{
}
template<typename Function,typename Action>
template<typename T>
Action forwarding_iterable_value_callable<Function,Action>::operator()(T&& i_value) const
{
	eval(m_function,std::forward<T>(i_value));

	return go_next_place;
}

}
