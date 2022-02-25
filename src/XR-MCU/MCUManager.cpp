#include "MCUManager.h"
#include "TcpSocketPlayerReceiver.h"
#include "MCUFusionManager.h"
#include "ConfigExceptions.h"
#include "ResourceManager.h"

DDK_APPEND_GLOBAL_ALLOCATOR(128,256)

namespace mcu
{
namespace
{

void setup_ntp_date()
{

}

}

MCUManager::MCUManager(const std::string& i_configFile)
: m_stop(false)
, m_configProvider(i_configFile)
{
	pthread_mutex_init(&m_conVarMutex,nullptr);
	pthread_cond_init(&m_condVar,nullptr);

	m_playerListener.sig_onPlayerCreated.connect(ddk::make_function(this,&MCUManager::onPlayerCreated));
	m_playerListener.sig_onPlayerUpdated.connect(ddk::make_function(this,&MCUManager::onPlayerUpdated));
	m_playerListener.sig_onPlayerDeleted.connect(ddk::make_function(this,&MCUManager::onPlayerDeleted));
}
MCUManager::~MCUManager()
{
	pthread_cond_destroy(&m_condVar);
	pthread_mutex_destroy(&m_conVarMutex);
}
void MCUManager::Init()
{
	const McuConfig mcuConfig = m_configProvider.getMCUConfig();

	DDK_LOG_INFO("Aqcuiring system time");
	setup_ntp_date();

	DDK_LOG_INFO("MCU Manager initiated");

	McuConfig::const_receiver_iterator itReceiver = mcuConfig.begin_receiver_types();
	for (; itReceiver != mcuConfig.end_receiver_types(); ++itReceiver)
	{
		try
		{
			m_playerListener.register_listener(*itReceiver,m_configProvider.getReceiverConfigProvider());
		}
		catch (const listener_bad_register& i_excp)
		{
			DDK_FAIL("Error while registering listener: " + std::string(i_excp.what()));
		}
	}

	try
	{
		m_playerContainer = ddk::make_unique_reference<PlayerContainer>(m_configProvider.getFrameReaderConfigProvider());

		m_fusionMgr = ddk::make_unique_reference<MCUFusionManager>(m_configProvider,ddk::promote_to_ref(ddk::lend(m_playerContainer)));

		m_decoderBank = ddk::make_unique_reference<FrameDecoderBank>(m_configProvider.getDecoderConfigProvider(),ddk::promote_to_ref(ddk::lend(m_fusionMgr)));

		m_decoderBank->init();

		m_measuremtManager.Init();

		ResourceManager::get_instance()->resgiter_resource_consumer(ddk::promote_to_ref(ddk::lend(m_decoderBank)));
	}
	catch (const bad_config& i_excp)
	{
		DDK_FAIL("Error during config retrieval: " + std::string(i_excp.what()));
	}
}
int MCUManager::Exec()
{
	m_fusionMgr->start();

	m_playerListener.start();

	pthread_mutex_lock(&m_conVarMutex);

	while (m_stop == false)
	{
		const std::chrono::seconds secs(1);
		const struct timespec time_to_wait = {time(NULL), std::chrono::duration_cast<std::chrono::nanoseconds>(secs).count()};

		pthread_cond_timedwait(&m_condVar,&m_conVarMutex,&time_to_wait);
	}

	m_measuremtManager.Deinit();

	m_playerContainer->Shutdown();

	m_playerListener.stop();

	pthread_mutex_unlock(&m_conVarMutex);

	return 0;
}
void MCUManager::close()
{
	pthread_mutex_lock(&m_conVarMutex);

	m_stop = true;

	pthread_cond_signal(&m_condVar);

	pthread_mutex_unlock(&m_conVarMutex);
}
void MCUManager::onPlayerCreated(player_id i_id, const PlayerCreateMessage& i_msg)
{
	if (m_playerContainer)
	{
		const McuConfig mcuConfig = m_configProvider.getMCUConfig();
		PlayerContainer::create_player_result createRes = m_playerContainer->CreatePlayer(i_id,i_msg,mcuConfig.get_field_of_view_config());

		if (createRes)
		{
			iplayer_lent_ref newPlayer = createRes.get();
			const frame_source_id frameSourceId(i_id.getValue());
			decoder_context regContext{ i_msg.get_decoder_type() };

			FrameDecoderBank::register_result regRes = m_decoderBank->register_frame_source(frameSourceId,regContext,newPlayer);

			if (regRes)
			{
				const McuConfig mcuConfig = m_configProvider.getMCUConfig();
				const FrameWriterConfig frameWriterConfig = m_configProvider.getFrameWriterConfigProvider().getConfig();
				encoder_context regContext{ i_msg.get_encoder_type(), i_msg.get_writer_type(), frameWriterConfig.getExchangeName(), frameWriterConfig.getUrl(), mcuConfig.get_num_encoder_trheads(), mcuConfig.get_max_num_decoded_pending_frames() };

				IFusionManager::register_result regRes = m_fusionMgr->register_frame_source_id(frameSourceId,newPlayer,regContext);

				if (regRes)
				{
					DDK_LOG_INFO("Player started " << i_id.getValue());

					newPlayer->start();
				}
				else
				{
					DDK_LOG_ERROR("Player could not be started: could not register frame receiver");
				}
			}
			else
			{
				DDK_LOG_ERROR("Player could not be registered to decoder bank");
			}
		}
		else
		{
			DDK_FAIL("Error while creating player");
		}
	}
	else
	{
		DDK_FAIL("Attempting to create player into null player container");
	}
}
void MCUManager::onPlayerUpdated(player_id i_id, const PlayerUpdateMessage& i_msg)
{
	if (m_playerContainer)
	{
		PlayerContainer::update_player_result updateRes = m_playerContainer->UpdatePlayer(i_id,i_msg);

		DDK_ASSERT(updateRes, "Error while creating player");
	}
	else
	{
		DDK_FAIL("Attempting to create player into null player container");
	}
}
void MCUManager::onPlayerDeleted(player_id i_id)
{
	if (m_playerContainer)
	{
		const frame_source_id id(i_id.getValue());

		IFusionManager::unregister_result unregSourceRes = m_fusionMgr->unregister_frame_source_id(id);

		DDK_ASSERT_OR_LOG(unregSourceRes, "Error unregistering player from fusion manager");

		FrameDecoderBank::unregister_result unregRes = m_decoderBank->unregister_frame_source(id);

		DDK_ASSERT_OR_LOG(unregRes, "Error unregistering player from decoder bank");

		PlayerContainer::delete_player_result deleteRes = m_playerContainer->DeletePlayer(i_id);

		DDK_ASSERT(deleteRes, "Error while creating player");
	}
	else
	{
		DDK_FAIL("Attempting to create player into null player container");
	}
}

}