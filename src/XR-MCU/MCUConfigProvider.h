#pragma once

#include "MCUConfig.h"
#include "rapidjson/document.h"

namespace mcu
{

struct MCUReceiverConfigProvider
{
public:
	MCUReceiverConfigProvider(const rapidjson::Document& i_doc);
	OrchestratorConfig getOrchestratorConfig() const;

private:
	const rapidjson::Document& m_document;
};

struct MCUFrameReaderConfigProvider
{
public:
	MCUFrameReaderConfigProvider(const rapidjson::Document& i_doc);
	FrameReaderConfig getConfig(FrameReaderType) const;

private:
	const rapidjson::Document& m_document;
};

struct MCUDecoderConfigProvider
{
public:
	MCUDecoderConfigProvider(const rapidjson::Document& i_doc);
	DecoderBanckConfig getConfig() const;

private:
	const rapidjson::Document& m_document;
};

struct MCUFrameWriterConfigProvider
{
public:
	MCUFrameWriterConfigProvider(const rapidjson::Document& i_doc);
	FrameWriterConfig getConfig() const;

private:
	const rapidjson::Document& m_document;
};

struct McuConfigProvider
{
public:
	McuConfigProvider(const std::string& i_configFile);

	McuConfig getMCUConfig() const;
	MCUReceiverConfigProvider getReceiverConfigProvider() const;
	MCUFrameReaderConfigProvider getFrameReaderConfigProvider() const;
	MCUDecoderConfigProvider getDecoderConfigProvider() const;
	MCUFrameWriterConfigProvider getFrameWriterConfigProvider() const;
	
private:
	rapidjson::Document m_document;
};

}