#include "FrameEncoderCache.h"
#include "ddk_lock_guard.h"

namespace mcu
{

ddk::shared_future<encoded_frame_dist_ref> FrameEncoderCache::get(const frame_souce_time_stamp_list& i_srcTimeStamps)
{
	static const frame_souce_time_stamp_list_less_operator timeStampLessComparator;

	const size_t idListHash = get_ids_hash(i_srcTimeStamps);
	const mcu_frame_time_point_list timeStampList = get_time_stamps(i_srcTimeStamps);

	ddk::mutex_guard mg(m_mutex);

	cache_container::const_iterator itCache = m_cache.find(idListHash);

	if(itCache != m_cache.end() && timeStampLessComparator(itCache->second.m_timeStampList,timeStampList) == false)
	{
		return itCache->second.m_future;
	}
	else
	{
		return {};
	}
}
void FrameEncoderCache::set(const frame_souce_time_stamp_list& i_srcTimeStamps, const ddk::shared_future<encoded_frame_dist_ref>& i_future)
{
	static const frame_souce_time_stamp_list_less_operator timeStampLessComparator;

	const size_t idListHash = get_ids_hash(i_srcTimeStamps);
	const mcu_frame_time_point_list timeStampList = get_time_stamps(i_srcTimeStamps);

	ddk::mutex_guard mg(m_mutex);

	cache_container::iterator itCache = m_cache.find(idListHash);

	if(itCache != m_cache.end())
	{
		if(timeStampLessComparator(itCache->second.m_timeStampList,timeStampList))
		{
			itCache->second = { timeStampList,i_future };
		}
	}
	else
	{
		m_cache.insert({ idListHash,{timeStampList,i_future} });
	}
}

}