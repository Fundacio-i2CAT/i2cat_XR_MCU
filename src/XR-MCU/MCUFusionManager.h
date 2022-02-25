#pragma once

#include "IFusionManager.h"
#include "MCUConfigProvider.h"
#include "MCUConfig.h"
#include "IPlayerContainer.h"
#include "ddk_lock_free_stack.h"
#include "ddk_thread_executor.h"
#include "FusionContainer.h"
#include "IFusionCollection.h"
#include "FrameEncoderDefs.h"
#include "FrameEncoderBank.h"

namespace mcu
{

class MCUFusionManager : public IFusionManager
{
public:
	MCUFusionManager(const McuConfigProvider& i_configProvider, player_provider_const_lent_ref i_playerProvider);
	~MCUFusionManager();


private:
	void start() override;
	register_result register_frame_source_id(frame_source_id i_id, iphysical_object_const_lent_ref i_obj, const encoder_context& i_context) override;
	unregister_result unregister_frame_source_id(frame_source_id) override;
	void enqueueDecodedFrame(frame_source_id i_sourceId, DecodedFrame i_frame) override;

	const McuConfig m_mcuConfig;
	player_provider_const_lent_ref m_playerProvider;
	FusionContainer m_fusionCtr;
	const FrameWriterConfig m_frameWriterConfig;
	ifusion_collection_unique_ptr m_fusionCollection;
};

}