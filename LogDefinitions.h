#ifndef __LogDefinitions_H__
#define __LogDefinitions_H__

#include <string>

namespace qlog
{
#define LOGGER_PTR_GLOBAL "g_QLoggerSingleton"

#define INFRA_LOG_LEVEL_UTIL         "UTIL"
#define INFRA_LOG_LEVEL_FATAL		 "FATAL"
#define INFRA_LOG_LEVEL_WARNING      "WARN"
#define INFRA_LOG_LEVEL_ERROR        "ERROR"
#define INFRA_LOG_LEVEL_INFO         "INFO"
#define INFRA_LOG_LEVEL_DEBUG        "DEBUG"




	enum class QuickLogLevel : int
	{
		log_fatal = 5,
		log_error = 4,
		log_warning = 3,
		log_info = 2,
		log_debug = 1,
		log_util = 0
	};



	static std::string log_level_to_string(QuickLogLevel level)
	{
		switch (level)
		{
		case QuickLogLevel::log_fatal:
			return INFRA_LOG_LEVEL_FATAL;
		case QuickLogLevel::log_warning:
			return INFRA_LOG_LEVEL_WARNING;
		case QuickLogLevel::log_error:
			return INFRA_LOG_LEVEL_ERROR;
		case QuickLogLevel::log_info:
			return INFRA_LOG_LEVEL_INFO;
		case QuickLogLevel::log_debug:
			return INFRA_LOG_LEVEL_DEBUG;
		default:
			return INFRA_LOG_LEVEL_UTIL;
		}
	}

}

#endif //__LogDefinitions_H__