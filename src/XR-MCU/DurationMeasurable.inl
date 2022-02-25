
namespace pfr
{

template<typename T, typename Ratio>
DurationMeasurable<T,Ratio>::DurationMeasurable(value_type& i_duration, profiler_time_duration i_timeBetweenMeasures)
: pfr::Measurable<std::chrono::duration<int,Ratio>,T>(i_timeBetweenMeasures)
, m_duration(i_duration)
{
}
template<typename T,typename Ratio>
typename DurationMeasurable<T,Ratio>::value_type DurationMeasurable<T,Ratio>::private_measure(const profiler_time_point&) const
{
	return m_duration;
}

}