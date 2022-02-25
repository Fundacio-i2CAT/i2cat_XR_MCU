#pragma once

#include "FrameDefs.h"
#include "ddk_type_id.h"
#include "MCUConfig.h"

namespace mcu
{

struct fusion_container_t;
typedef ddk::Id<uint8_t,fusion_container_t> fusion_container_id;

typedef std::vector<std::pair<frame_source_id,mcu_frame_time_point>> frame_souce_time_stamp_list;

frame_source_list_id get_ids(const frame_souce_time_stamp_list& i_idList);
size_t get_ids_hash(const frame_souce_time_stamp_list& i_idList);
mcu_frame_time_point_list get_time_stamps(const frame_souce_time_stamp_list& i_idList);

}