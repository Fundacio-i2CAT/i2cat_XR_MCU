#pragma once

#include "EncodedFrame.h"
#include "FusionedFrame.h"
#include "ddk_async.h"
#include "ddk_hash.h"
#include <map>

namespace mcu
{

class FrameEncoderCache
{
	struct EncodedFrameFuture
	{
	
		mcu_frame_time_point_list m_timeStampList;
		ddk::shared_future<encoded_frame_dist_ref> m_future;
	};

	typedef std::map<size_t,EncodedFrameFuture> cache_container;

public:
	FrameEncoderCache() = default;

	ddk::shared_future<encoded_frame_dist_ref> get(const frame_souce_time_stamp_list& i_srcTimeStamps);
	void set(const frame_souce_time_stamp_list& i_srcTimeStamps, const ddk::shared_future<encoded_frame_dist_ref>& i_future);

private:
	cache_container m_cache;
	ddk::mutex m_mutex;
};

}