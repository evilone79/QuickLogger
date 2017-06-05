#include "DebugTraceSink.h"
#include <sstream>
#include <Windows.h>
#include "LogItem.h"
#include "Utils.hpp"


void qlog::DebugTraceSink::handle_log_item(const LogItem* logItem)
{
	std::stringstream ss;	
	util::time_point_to_string(logItem->m_logTime, ss);
	ss << " TID:" << logItem->m_tid << " " << logLevelToString(logItem->m_level)
		<< " " << logItem->m_message << " in: " << logItem->m_function << ", " << logItem->m_file << ", "
		<< logItem->m_line << "\n";
		
	OutputDebugStringA(ss.str().c_str());
}
