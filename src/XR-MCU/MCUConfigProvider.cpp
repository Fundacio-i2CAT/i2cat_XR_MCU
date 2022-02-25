#include "MCUConfigProvider.h"
#include "ddk_formatter.h"
#include <fstream>
#include "rapidjson/filereadstream.h"
#include <set>
#include "ConfigExceptions.h"

namespace mcu
//adding SocketIO here
{

MCUReceiverConfigProvider::MCUReceiverConfigProvider(const rapidjson::Document& i_doc)
: m_document(i_doc)
{
}
OrchestratorConfig MCUReceiverConfigProvider::getOrchestratorConfig() const
{
	ConnectionConfig::Type connType;
	std::string connIp;
	unsigned int connPort;

	rapidjson::Value::ConstMemberIterator itMember = m_document.FindMember("Orchestrator");
	if (itMember != m_document.MemberEnd())
	{
		const auto& orchConfig = itMember->value;

		rapidjson::Value::ConstMemberIterator itOrchMember = orchConfig.FindMember("Connection");
		if(itOrchMember != orchConfig.MemberEnd())
		{
			const auto& connConfig = itOrchMember->value;
		
			if(connConfig.HasMember("Type"))
			{
				connType = static_cast<ConnectionConfig::Type>(connConfig["Type"].GetUint());
			}
			else
			{
				throw bad_config("Could not retrieve orchestrator connection config type");
			}

			if(connConfig.HasMember("Ip"))
			{
				connIp = connConfig["Ip"].GetString();
			}
			else
			{
				throw bad_config("Could not retrieve orchestrator connection ip");
			}

			if(connConfig.HasMember("Port"))
			{
				connPort = connConfig["Port"].GetUint();
			}
			else
			{
				throw bad_config("Could not retrieve orchestrator connection port");
			}
		}
		else
		{
			throw bad_config("Could not retrieve tcp receiver config section");
		}
	}
	else
	{
		throw bad_config("Could not retrieve tcp receiver config section");
	}

	return ConnectionConfig{connType,connIp,connPort};
}

MCUFrameReaderConfigProvider::MCUFrameReaderConfigProvider(const rapidjson::Document& i_doc)
: m_document(i_doc)
{
}
FrameReaderConfig MCUFrameReaderConfigProvider::getConfig(FrameReaderType) const
{
	size_t frameRate;

	rapidjson::Value::ConstMemberIterator itMember = m_document.FindMember("FrameReader");
	if(itMember != m_document.MemberEnd())
	{
		auto& obj = itMember->value;

		if(obj.HasMember("FrameRate"))
		{
			frameRate = obj["FrameRate"].GetUint();
		}
		else
		{
			throw bad_config("Could not retrieve frame rate config");
		}
	}
	else
	{
		throw bad_config("Could not retrieve frame reader config section");
	}

	return FrameReaderConfig(frameRate);
}

MCUDecoderConfigProvider::MCUDecoderConfigProvider(const rapidjson::Document& i_doc)
: m_document(i_doc)
{
}
DecoderBanckConfig MCUDecoderConfigProvider::getConfig() const
{
	unsigned int numThreadsPerDecoder, maxNumPendingFrames;

	rapidjson::Value::ConstMemberIterator itMember = m_document.FindMember("Decoder");
	if (itMember != m_document.MemberEnd())
	{
		auto& obj = itMember->value;

		if (obj.HasMember("MaxNumThreadsPerDecoder"))
		{
			numThreadsPerDecoder = obj["MaxNumThreadsPerDecoder"].GetUint();
		}
		else
		{
			throw bad_config("Could not retrieve decoder config MaxNumThreadsPerDecoder");
		}

		if (obj.HasMember("MaxNumPendingFrames"))
		{
			maxNumPendingFrames = obj["MaxNumPendingFrames"].GetUint();
		}
		else
		{
			throw bad_config("Could not retrieve decoder config MaxNumPendingFrames");
		}
	}
	else
	{
		throw bad_config("Could not retrieve tcp receiver config section");
	}

	return DecoderBanckConfig(numThreadsPerDecoder,maxNumPendingFrames);
}

MCUFrameWriterConfigProvider::MCUFrameWriterConfigProvider(const rapidjson::Document& i_doc)
: m_document(i_doc)
{
}
FrameWriterConfig MCUFrameWriterConfigProvider::getConfig() const
{
	std::string url, exchangeName;

	rapidjson::Value::ConstMemberIterator itMember = m_document.FindMember("FrameWriter");
	if (itMember != m_document.MemberEnd())
	{
		auto& obj = itMember->value;

		if (obj.HasMember("Url"))
		{
			url = obj["Url"].GetString();
		}
		else
		{
			throw bad_config("Could not retrieve frame retriever config Url");
		}

		if (obj.HasMember("ExchangeName"))
		{
			exchangeName = obj["ExchangeName"].GetString();
		}
		else
		{
			throw bad_config("Could not retrieve frame writer config ExchangeName");
		}
	}
	else
	{
		throw bad_config("Could not retrieve frame writer config section");
	}

	return FrameWriterConfig(url,exchangeName);
}


McuConfigProvider::McuConfigProvider(const std::string& i_configFile)
{
	if (FILE* fp = fopen(i_configFile.c_str(), "rb"))
	{
		char readBuffer[4096];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		try
		{
			m_document.ParseStream(is);

			DDK_ASSERT(m_document.HasParseError() == false, "Error parsing file" + i_configFile);
		}
		catch(...)
		{
			throw bad_config("Internal json error parsing file " + i_configFile);
		}
	}
	else
	{
		DDK_FAIL("Config file does not exists");
	}
}
McuConfig McuConfigProvider::getMCUConfig() const
{
	std::set<ReceiverType> receiverTypes;
	unsigned int frameRate = 0;
	size_t maxNumPendingFrames = 0;
	size_t numThreadsPerEncoder = 0;
	size_t numEncodedFramesToSent = 0;
	float primaryDistance = 0.f;
	unsigned int mainFov = 0;
	unsigned int totalFov = 0;

	rapidjson::Value::ConstMemberIterator itMember = m_document.FindMember("MCU");
	if (itMember != m_document.MemberEnd())
	{
		auto& obj = itMember->value;

		if (obj.HasMember("ReceiverList"))
		{
			const std::string receiverList = obj["ReceiverList"].GetString();

			const std::set<std::string> receiverStrTypes = ddk::formatter<std::set<std::string>>::format(receiverList);
			std::set<std::string>::const_iterator itReceiverStr = receiverStrTypes.begin();
			for (; itReceiverStr != receiverStrTypes.end(); ++itReceiverStr)
			{
				ReceiverType receiverType;
				if (get_enum_from_name(receiverType, itReceiverStr->c_str()))
				{
					receiverTypes.insert(receiverType);
				}
			}
		}
		if (obj.HasMember("MaxNumPendingDecodedFrames"))
		{
			maxNumPendingFrames = obj["MaxNumPendingDecodedFrames"].GetUint();
		}
		if (obj.HasMember("NumEncodedFramesToSent"))
		{
			numEncodedFramesToSent = obj["NumEncodedFramesToSent"].GetUint();
		}
		if (obj.HasMember("MaxNumThreadsPerEncoder"))
		{
			numThreadsPerEncoder = obj["MaxNumThreadsPerEncoder"].GetUint();
		}
		if (obj.HasMember("FrameRate"))
		{
			frameRate = obj["FrameRate"].GetUint();
		}
	}

	return { receiverTypes,maxNumPendingFrames,numEncodedFramesToSent,numThreadsPerEncoder,frameRate,primaryDistance,mainFov,totalFov };
}
MCUReceiverConfigProvider McuConfigProvider::getReceiverConfigProvider() const
{
	return m_document;
}
MCUFrameReaderConfigProvider McuConfigProvider::getFrameReaderConfigProvider() const
{
	return m_document;
}
MCUDecoderConfigProvider McuConfigProvider::getDecoderConfigProvider() const
{
	return m_document;
}
MCUFrameWriterConfigProvider McuConfigProvider::getFrameWriterConfigProvider() const
{
	return m_document;
}

}