#pragma once

#include <functional>
#include <sstream>

template<int level>
class ddk_logger
{
public:
	template<class logger_t>
	static void configure(logger_t logger) { log = logger; }

	static bool enabled() { return bool(log); }

	~ddk_logger() { log(stream.str()); }

	template<class value_t>
	ddk_logger& operator<<(value_t value) { stream << value; return *this; }

private:
	static std::function<void(const std::string &)> log;

	std::ostringstream stream;
};

enum LogLevel { LOG_LEVEL_DEBUG, LOG_LEVEL_INFO, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR, LOG_LEVEL_FATAL };

#define DDK_LOG_DEBUG(message) if(!ddk_logger<LOG_LEVEL_DEBUG>::enabled()) {} else ddk_logger<LOG_LEVEL_DEBUG>() << message
#define DDK_LOG_INFO(message) if(!ddk_logger<LOG_LEVEL_INFO>::enabled()) {} else ddk_logger<LOG_LEVEL_INFO>() << message
#define DDK_LOG_WARN(message) if(!ddk_logger<LOG_LEVEL_WARNING>::enabled()) {} else ddk_logger<LOG_LEVEL_WARNING>() << message
#define DDK_LOG_ERROR(message) if(!ddk_logger<LOG_LEVEL_ERROR>::enabled()) {} else ddk_logger<LOG_LEVEL_ERROR>() << message
#define DDK_LOG_FATAL(message) if(!ddk_logger<LOG_LEVEL_FATAL>::enabled()) {} else ddk_logger<LOG_LEVEL_FATAL>() << message
