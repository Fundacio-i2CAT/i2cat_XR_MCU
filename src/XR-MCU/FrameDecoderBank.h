#pragma once

#include "IRawFrameSource.h"
#include "IDecodedFrameReceiver.h"
#include "IRawFrameDecoder.h"
#include "FrameDecoderDefs.h"
#include "MCUConfigProvider.h"
#include "ddk_thread_pool.h"
#include "ProfilerMeasure.h"
#include "ICpuResource.h"
#include "ddk_result.h"
#include <map>
#include <set>

namespace mcu
{

class FrameDecoderBank : public ICpuResource
{
public:
	enum class RegisterErrorCode
	{
		RegisterSourceALreadyExist,
		RegisterUnsupportedDecoder,
		RegisterLackOfResources,
		RegisterInternalError
	};
	typedef ddk::result<void,RegisterErrorCode> register_result;
	enum class UnregisterErrorCode
	{
		UnregisterSourceNonExist
	};
	typedef ddk::result<void,UnregisterErrorCode> unregister_result;

	FrameDecoderBank(const MCUDecoderConfigProvider& i_configProvider, idecoded_frame_receiver_lent_ref i_frameReceiver);
	FrameDecoderBank(const FrameDecoderBank&) = delete;
	~FrameDecoderBank();

	void init();
	register_result register_frame_source(frame_source_id i_id, const decoder_context& i_context, iraw_frame_source_const_lent_ref i_frameSource);
	unregister_result unregister_frame_source(frame_source_id i_id);

private:
	size_t needed_resources() const override;
	void set_affinity(const cpu_set_t&) override;

	void checkDecoderFrameRate(const frame_decoder_fps& i_measure);

	std::map<frame_source_id,iraw_frame_decoder_unique_ref> m_decoders;
	ddk::thread_pool m_threadPool;
	DecoderBanckConfig m_config;
	idecoded_frame_receiver_lent_ref m_frameReceiver;
	std::vector<ddk::connection> m_measuremntConn;
	pthread_mutex_t m_decoderMutex;
	std::set<frame_decoder_fps> m_decoderFpsSet;
	frame_source_id m_currmaxFpsId;
};

}