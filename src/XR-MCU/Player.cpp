#include "Player.h"
#include "ddk_optional.h"
#include "DecodedFrame.h"
#include "SocketIoPlayerReceiver.h"
#include "rapidjson/document.h"

namespace mcu
{

const size_t k_updatePlayerEvent = 113;

Player::Player(const player_id& i_id, const FrameReaderConfig& i_config, const FieldOfViewConfig& i_fovConfig)
: m_id(i_id.getValue())
, m_updateExecutor(std::chrono::milliseconds(i_config.get_polling_time()))
, m_frustrum(i_fovConfig.get_primary_area_distance(),i_fovConfig.get_main_fov(),i_fovConfig.get_total_fov())
{
}
void Player::init(frame_reader_unique_ref i_frameReader, const PlayerData& i_msg)
{
	m_frameReader = std::move(i_frameReader);
	m_url = i_msg.m_url;
	m_transform.set_position(i_msg.m_pos);
	m_transform.set_rotation(i_msg.m_rotation);
	m_publicUrl = i_msg.m_url;
}
void Player::notify() const
{
	sig_onUpdate.execute();
}
void Player::start()
{
	if (m_frameReader)
	{
		IPlayerReceiver::get_instance()->listen_event("SceneEventToUser",ddk::make_function(this,&Player::onPlayerChanged));

		m_updateExecutor.start_thread(ddk::make_function(this,&Player::checkForNewFrames));
	}
	else
	{
		DDK_FAIL("Trying to start player thread without reader associated");
	}
}
void Player::update(const PlayerUpdateMessage& i_msg)
{
	if (i_msg.visit(*this))
	{
		sig_onUpdate.execute();
	}
}
void Player::stop()
{
	m_updateExecutor.stop_thread();
}
const transformation& Player::get_transformation() const
{
	return m_transform;
}
Frustrum Player::get_frustrum() const
{
	return m_frustrum;
}
bool Player::operator()(const std::array<float, 3>& i_pos)
{
	m_transform.set_position(i_pos);

	return true;
}
bool Player::operator()(float i_rot)
{
	m_transform.set_rotation(i_rot);

	return true;
}
bool Player::operator()(Frustrum i_frustrum)
{
	m_frustrum = i_frustrum;

	return true;
}
bool Player::operator()(const std::string& i_url)
{
	m_publicUrl = i_url;

	return true;
}
void Player::checkForNewFrames()
{
	if (m_frameReader->is_connected())
	{
		IFrameReader::read_result readRes = m_frameReader->read();

		if (readRes)
		{
			sig_onNewFrame.execute(std::move(readRes).extract());
		}
	}
	else
	{
		IFrameReader::connect_result connectRes = m_frameReader->connect();

		DDK_ASSERT_OR_LOG(connectRes,"Player could not get connected");
	}
}
bool Player::onPlayerChanged(const MessageResponse& i_event)
{
	bool somethingChanged = false;
	bool captured = false;

	MessageResponse::const_iterator itEntry = i_event.begin();
	for(;itEntry!=i_event.end();++itEntry)
	{
		if(itEntry->has_key("sceneEventData"))
		{
			std::string contents = itEntry->get_value("sceneEventData");
			
			//remove strange initial character
			contents.erase(contents.begin());

			rapidjson::Document document;

			document.Parse(contents.c_str());

			if(document.HasParseError() == false)
			{
				if(document.HasMember("TypeId"))
				{
					if(document["TypeId"].GetInt() == k_updatePlayerEvent)
					{
						const std::string data = document["Data"].GetString();

						rapidjson::Document ddocument;
						ddocument.Parse(data.c_str());

						if(ddocument.HasParseError() == false)
						{
							if(ddocument.HasMember("SenderId") && ddocument["SenderId"].GetString() == m_id)
							{
								captured = true;

								//update position
								if(ddocument.HasMember("BodyPosition"))
								{
									const rapidjson::Value& bodyPos = ddocument["BodyPosition"];

									const bool posChanged = m_transform.set_position({ static_cast<float>(bodyPos["x"].GetDouble()), static_cast<float>(bodyPos["y"].GetDouble()), static_cast<float>(bodyPos["z"].GetDouble()) });
									somethingChanged |= posChanged;

									if(posChanged)
									{
										DDK_LOG_INFO("new pos " << static_cast<float>(bodyPos["x"].GetDouble()) << ", " << static_cast<float>(bodyPos["y"].GetDouble()) << ", " << static_cast<float>(bodyPos["z"].GetDouble()) << " from " << m_id);
									}
								}

								//update rotation
								if(ddocument.HasMember("BodyRotation"))
								{
									const rapidjson::Value& bodyRot = ddocument["BodyRotation"];

									const bool rotChanged = m_transform.set_rotation( { static_cast<float>(bodyRot["w"].GetDouble()),static_cast<float>(bodyRot["x"].GetDouble()),static_cast<float>(bodyRot["y"].GetDouble()),static_cast<float>(bodyRot["z"].GetDouble()) });
									somethingChanged |= rotChanged;
								}


								//update frustrum
								if(ddocument.HasMember("CameraPosition") && ddocument.HasMember("CameraRotation"))
								{
									const rapidjson::Value& cameraPos = ddocument["CameraPosition"];
									const rapidjson::Value& cameraRot = ddocument["CameraRotation"];

									const bool frustrumChanged = m_frustrum.update({ static_cast<float>(cameraPos["x"].GetDouble()),static_cast<float>(cameraPos["y"].GetDouble()), static_cast<float>(cameraPos["z"].GetDouble()) },{ static_cast<float>(cameraRot["w"].GetDouble()),static_cast<float>(cameraRot["x"].GetDouble()),static_cast<float>(cameraRot["y"].GetDouble()),static_cast<float>(cameraRot["z"].GetDouble()) });
									somethingChanged |= frustrumChanged;
								}
							}
						}
					}
				}
			}
		}
	}

	if(somethingChanged)
	{
		sig_onUpdate.execute();
	}

	return captured;
}

}