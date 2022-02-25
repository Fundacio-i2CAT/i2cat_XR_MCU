#pragma once

#include "OperationsRateMeasurable.h"
#include "DurationMeasurable.h"
#include "FunctorMeasurable.h"
#include "FrameDecoderDefs.h"
#include "FrameEncoderDefs.h"

namespace mcu
{

extern const pfr::profile_module_id k_fusion_profile_module;

struct dropped_frame_ratio_t;
typedef pfr::FunctorMeasurable<float,dropped_frame_ratio_t> DroppedDecodedFrameRatio;

struct fusion_time_t;
typedef pfr::DurationMeasurable<fusion_time_t,std::milli> FusionTimeMeasurable;
typedef FusionTimeMeasurable::value_type fusion_duration_time;

struct fusioned_frame_latency_t;
typedef pfr::FunctorMeasurable<frame_latency,fusioned_frame_latency_t> FusionedFrameLatencyMeasurable;

struct fusioned_frame_rate_t;
typedef pfr::FunctorMeasurable<frame_fusioned_fps,fusioned_frame_rate_t> FusionedFrameRateMeasurable;

struct sent_frame_rate_t;
typedef pfr::FunctorMeasurable<frame_sent_fps,sent_frame_rate_t> SentFrameRateMeasurable;

struct encoding_time_t;
typedef pfr::FunctorMeasurable<encoding_time,encoding_time_t> EncodingTimeMeasurable;
typedef EncodingTimeMeasurable::value_type encoding_duration_time;

struct sending_time_t;
typedef pfr::DurationMeasurable<sending_time_t,std::milli> SendingTimeMeasurable;
typedef SendingTimeMeasurable::value_type sending_duration_time;

struct sent_total_bytes_rate_t;
typedef pfr::OperationsRateMeasurable<sent_total_bytes_rate_t> SentTotalBytesRateMeasurable;

}