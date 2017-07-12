#include "QuickLogger.h"
#include "DebugTraceSink.h"
#include "FileSink.h"



void qlog::QuickLogger::log(QuickLogLevel level, std::string&& file, std::string&& function, std::string&& msg, int line)
{
	auto logItem = m_logManager.prepare_log_item();
	logItem->m_file = std::move(file);
	logItem->m_function = std::move(function);
	logItem->m_level = level;
	logItem->m_line = line;
	logItem->m_message = std::move(msg);
	if (level >= m_logLevel)
	{
		m_logManager.submit_log_item(logItem);
	}
}






