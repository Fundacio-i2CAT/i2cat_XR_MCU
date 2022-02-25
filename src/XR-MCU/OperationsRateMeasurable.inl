
namespace pfr
{

template<typename T>
OperationsRateMeasurable<T>::OperationsRateMeasurable(size_t& i_numFrameSinceLastEpoch, profiler_time_duration i_timeBetweenMeasures)
: Measurable<float,T>(i_timeBetweenMeasures)
, m_lastFramesSinceLastEpoch(i_numFrameSinceLastEpoch)
, m_lastUpdateTime(pfr::profiler_time_point{})
{
}
template<typename T>
float OperationsRateMeasurable<T>::private_measure(const profiler_time_point& i_time) const
{
	const float res = (m_lastUpdateTime != pfr::profiler_time_point{}) ? static_cast<float>(m_lastFramesSinceLastEpoch) / std::chrono::duration_cast<std::chrono::seconds>(i_time - m_lastUpdateTime).count() : m_lastFramesSinceLastEpoch;

	m_lastFramesSinceLastEpoch = 0;
	m_lastUpdateTime = i_time;

	return res;
}

}
