#ifndef __LogItem_H__
#define __LogItem_H__

#include "LogDefinitions.h"
#include <string>
#include <chrono>
#include <thread>
#include <atomic>

namespace qlog
{
	namespace chrono = std::chrono;

	using sys_clk = chrono::system_clock;
	using time_pt = chrono::time_point<sys_clk>;

	class LogItem
	{

	public:
		LogItem() = default;

		QuickLogLevel m_level;
		time_pt m_logTime;
		std::string m_file;
		std::string m_function;
		std::string m_message;
		unsigned int m_line;
		std::thread::id m_tid;
		std::atomic_size_t m_releaseCount = 0;

	};
}
#endif
