#pragma once

#include "IFrameReader.h"
#include "signals_unity_bridge.h"
#include "ProfilerDefs.h"

namespace mcu
{

class SubFrameReader : public IFrameReader
{
public:
	enum InitErrorCode
	{
		InitHandleCreateError,
		InitHandleAlreadyCreatedError
	};
	typedef ddk::result<void,InitErrorCode> init_result;

	SubFrameReader();
	~SubFrameReader();
	init_result init(const sub_reader_init_context& i_initContext);

private:
	connect_result connect() override;
	read_result read() override;
	destroy_result destroy() override;
	bool is_connected() const override;

	void reconnect();

	sub_handle* m_subHandle;	
	std::string m_url;
	ReaderId m_id;
	bool m_isPlaying;
	size_t m_streamCount;
	size_t m_numberOfUnsuccessfulReceives;
	size_t m_numReceivedFrames;
	pfr::measure_id m_ReceivedFrameRateMeasId;
};

}