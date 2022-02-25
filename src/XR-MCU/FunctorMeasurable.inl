
namespace pfr
{

template<typename Return, typename T>
FunctorMeasurable<Return, T>::FunctorMeasurable(const ddk::function<Return(const profiler_time_point&)>& i_functor, profiler_time_duration i_timeBetweenMeasures)
: Measurable<Return,T>(i_timeBetweenMeasures)
, m_functor(i_functor)
{
}
template<typename Return, typename T>
Return FunctorMeasurable<Return, T>::private_measure(const profiler_time_point& i_time) const
{
	return ddk::eval(m_functor,i_time);
}

}
