#pragma once

#include "FrameDefs.h"
#include "FrameDecoderDefs.h"
#include "ProfilerDefs.h"
#include "OperationsRateMeasurable.h"
#include "DurationMeasurable.h"
#include "FunctorMeasurable.h"
#include <utility>

namespace mcu
{

extern const pfr::profile_module_id k_acquisition_profile_module;

struct received_frame_rate_t;
typedef pfr::FunctorMeasurable<frame_receiver_fps,received_frame_rate_t> ReceivedFrameRateMeasurable;

struct received_total_bytes_rate_t;
typedef pfr::OperationsRateMeasurable<received_total_bytes_rate_t> ReceivedTotalBytesRateMeasurable;

struct received_pointcloud_size_t;
typedef pfr::FunctorMeasurable<pointcloud_size,received_pointcloud_size_t> PointcloudSizeMeasurable;

struct decoded_frame_latency_t;
typedef pfr::FunctorMeasurable<frame_latency,decoded_frame_latency_t> DecodedFrameLatencyMeasurable;

struct decoded_frame_rate_t;
typedef pfr::FunctorMeasurable<frame_decoder_fps,decoded_frame_rate_t> DecodedFrameRateMeasurable;

struct transformed_frame_rate_t;
typedef pfr::FunctorMeasurable<frame_decoder_fps,transformed_frame_rate_t> TransformedFrameRateMeasurable;

struct decoding_time_t;
typedef pfr::FunctorMeasurable<decoding_time, decoding_time_t> DecodingTimeMeasurable;
typedef DecodingTimeMeasurable::value_type decoding_duration_time;

struct transform_time_t;
typedef pfr::FunctorMeasurable<transform_time,transform_time_t> TransformTimeMeasurable;
typedef TransformTimeMeasurable::value_type transform_duration_time;

}