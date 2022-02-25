#include "FusionDefs.h"
#include "ddk_hash.h"

namespace mcu
{

frame_source_list_id get_ids(const frame_souce_time_stamp_list& i_idList)
{
	frame_source_list_id res;

	for(const auto& entry : i_idList)
	{
		res.insert(entry.first);
	}

	return res;
}

size_t get_ids_hash(const frame_souce_time_stamp_list& i_idList)
{
	size_t res = 0;

	const ddk::commutative_builtin_hasher s_hasher;

	for(const auto& entry : i_idList)
	{
		s_hasher(entry.first);
	}

	return s_hasher.get();
}

mcu_frame_time_point_list get_time_stamps(const frame_souce_time_stamp_list& i_idList)
{
	mcu_frame_time_point_list res;

	res.reserve(i_idList.size());

	for(const auto& entry : i_idList)
	{
		res.push_back(entry.second);
	}

	return res;
}

}