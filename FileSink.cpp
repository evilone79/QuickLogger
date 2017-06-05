#include "FileSink.h"


#include <filesystem>
#include <chrono>
#include <memory>
#include <vector>
#include "LogItem.h"
#include "LogDefinitions.h"
#include "Utils.hpp"

#include <sstream>


namespace fs = std::experimental::filesystem;
using namespace std::chrono;

qlog::FileSink::FileSink(const string& logFile) :
	m_logFileName {logFile}
{	
	set_log_file_name(m_logFileName);
}

qlog::FileSink::~FileSink(void) 
{	
	m_file.close();
}

void qlog::FileSink::handle_log_item(const LogItem* logItem)
{
	std::stringstream ss;
	
	util::time_point_to_string(logItem->m_logTime, ss);
	ss << " (" << logItem->m_tid << ") " << logLevelToString(logItem->m_level)
		<< " " << logItem->m_message << " in: " << logItem->m_function << ", " << logItem->m_file << ", "
		<< logItem->m_line << "\n";
	std::lock_guard<std::mutex> lock(m_fileMtx);
	if (!m_file)
	{
		m_file.open(m_logFileName, std::ios::out | std::ios::app);
	}
	if (m_file.good())
	{
		m_file << ss.rdbuf();
		m_file << std::flush;
		if (m_file.tellp() >= m_maxLogFileSize)
		{
			move_log_file_window();
		}
	}
}


void qlog::FileSink::move_log_file_window()
{	
	m_file.close();	
	std::ofstream tmp("log.tmp");
	std::ifstream ifs(m_logFileName);
	if (ifs && tmp)
	{
		ifs.seekg(m_logWindowSize);
		tmp << ifs.rdbuf();
		ifs.close();
		tmp.close();		
	}
	fs::rename("log.tmp", m_logFileName);
	m_file.open(m_logFileName, std::ios::out | std::ios::app);
	
}

void qlog::FileSink::set_log_file_name(const string& val)
{
	m_logFileName = val;
	std::lock_guard<std::mutex> lock(m_fileMtx);
	if (m_file.is_open())
	{
		m_file.close();
	}
	m_file.open(m_logFileName, std::ios::out | std::ios::app);
	if (!m_file)
	{
		string err = strerror(errno);
		throw std::runtime_error("Could not open log file. " + err);
	}

}
