#include "MCULogger.h"
#include "log4cxx/logger.h"
#include "log4cxx/xml/domconfigurator.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"
#include "DDKLogger.h"

namespace mcs
{

MCULogger::MCULogger(const std::string& i_logLevel)
{
	log4cxx::LoggerPtr  logger(log4cxx::Logger::getLogger(i_logLevel.c_str()));

	logger->setLevel(log4cxx::Level::getInfo());

	log4cxx::xml::DOMConfigurator::configure("config/Log4cxxConfig.xml");
	log4cxx::BasicConfigurator::configure();

	ddk_logger<LOG_LEVEL_INFO>::configure([logger](const std::string& i_str){ logger->info(i_str); });
	ddk_logger<LOG_LEVEL_WARNING>::configure([logger](const std::string& i_str){ logger->warn(i_str); });
	ddk_logger<LOG_LEVEL_ERROR>::configure([logger](const std::string& i_str){ logger->error(i_str); });
}


}