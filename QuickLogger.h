#ifndef __InfraLogger_H__
#define __InfraLogger_H__
#define BOOST_USE_WINDOWS_H 1

#include "LogManager.h"
#include "sstream"
#include <filesystem>

#include "DefaultFormatter.hpp"


namespace fs = std::experimental::filesystem;

namespace qlog
{
	class QuickLogger
	{
	public:

		static QuickLogger& instance()
		{
			static QuickLogger logger;
			return logger;
		}

		template <template<class F> class TSink, class Formatter = formatters::DefaultFormatter>
		TSink<Formatter>& append_sink()
		{
			static_assert(std::is_base_of_v<LogSink, TSink<Formatter>>, "Class does not derive from LogSink!");
			auto pSink = std::make_unique<TSink<Formatter>>();
			auto& refSink = *pSink;
			m_logManager.register_sink(std::move(pSink));	
			return refSink;
		}

		void set_log_level(QuickLogLevel level) { m_logLevel = level; }
		QuickLogLevel log_level() const { return m_logLevel; }

		void log(QuickLogLevel level, std::string&& file, std::string&& function, std::string&& msg, int line);


	private:
		QuickLogger() {}
		QuickLogLevel m_logLevel = qlog::QuickLogLevel::log_debug;
		LogManager m_logManager;		
	};
}



#define QuickLog qlog::QuickLogger::instance()

#define LOG(level_, message_){ \
	QuickLog.log(level_, \
	fs::path(__FILE__).filename().string(), \
	__FUNCTION__, static_cast<std::ostringstream&>(std::ostringstream().flush() << message_).str(), __LINE__); }

#define LOG_F(level_, format_, ...){ \
	constexpr size_t buffSize = 1024; \
	char msg[buffSize] = { 0 }; \
	auto count = _snprintf_s(msg, buffSize, _TRUNCATE, format_, __VA_ARGS__); \
	QuickLog.log(level_, fs::path(__FILE__).filename().string(), __FUNCTION__, msg, __LINE__ ); }


#define LogUtil(message_) LOG(qlog::QuickLogLevel::log_none, message_)
#define LogDebug(message_) LOG(qlog::QuickLogLevel::log_debug, message_)
#define LogInfo(message_) LOG(qlog::QuickLogLevel::log_info, message_)
#define LogWarning(message_) LOG(qlog::QuickLogLevel::log_warning, message_)
#define LogError(message_) LOG(qlog::QuickLogLevel::log_error, message_)
#define LogCritical(message_) LOG(qlog::QuickLogLevel::log_critical, message_)

#define LogUtilF(format_, ...) LOG_F(qlog::QuickLogLevel::log_none, format_, __VA_ARGS__)
#define LogDebugF(format_, ...) LOG_F(qlog::QuickLogLevel::log_debug, format_, __VA_ARGS__)
#define LogInfoF(format_, ...) LOG_F(qlog::QuickLogLevel::log_info, format_, __VA_ARGS__)
#define LogWarningF(format_, ...) LOG_F(qlog::QuickLogLevel::log_warning, format_, __VA_ARGS__)
#define LogErrorF(format_, ...) LOG_F(qlog::QuickLogLevel::log_error, format_, __VA_ARGS__)
#define LogCriticalF(format_, ...) LOG_F(qlog::QuickLogLevel::log_critical, format_, __VA_ARGS__)




#endif