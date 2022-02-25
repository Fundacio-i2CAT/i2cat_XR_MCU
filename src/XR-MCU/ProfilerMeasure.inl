
#include "ProfilerExceptions.h"

namespace pfr
{

template<typename T>
Measure::Measure(measure_type i_type, T&& i_value)
: m_type(i_type)
, m_arena(std::forward<T>(i_value))
{
}
template<typename T>
const T& Measure::get() const
{
	return m_arena.getValue<T>();
}

}