
namespace pfr
{

template<typename Return, typename T>
Measurable<Return, T>::Measurable(profiler_time_duration i_timeBetweenMeasures)
: m_lastMeasure(profiler_time_point())
, m_timeBetweenMeasures(i_timeBetweenMeasures)
{
}
template<typename Return, typename T>
void Measurable<Return, T>::bind(imeasurer_const_lent_ref i_measurer)
{
	m_measurer = i_measurer;
}
template<typename Return, typename T>
void Measurable<Return,T>::measure(const profiler_time_point& i_time) const
{
	if ((i_time - m_lastMeasure) >= m_timeBetweenMeasures)
	{
		Return meas = private_measure(i_time);
	
		m_lastMeasure = i_time;

		if (m_measurer)
		{
			m_measurer->onNewMeasure({ get_type(), meas });
		}
	}
}
template<typename Return, typename T>
measure_type Measurable<Return,T>::get_type()
{
	static const measure_type s_type(ddk::getTypeId<T>());

	return s_type;
}

}