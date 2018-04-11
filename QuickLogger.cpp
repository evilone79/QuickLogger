#include "QuickLogger.h"
#include "DebugTraceSink.h"
#include "FileSink.h"



void qlog::QuickLogger::log(QuickLogLevel level, std::string&& file, std::string&& function, std::string&& msg, int line)
{	
	if (level >= m_logLevel)
	{
		auto slot = m_logManager.prepare_log_item();
		auto logItem = slot->item();
		logItem->m_file = std::move(file);
		logItem->m_function = std::move(function);
		logItem->m_level = level;
		logItem->m_line = line;
		logItem->m_message = std::forward<std::string>(msg);
		m_logManager.submit_log_item(slot);
	}
}






