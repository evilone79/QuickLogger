#ifndef __FileSink_H__
#define __FileSink_H__
#define BOOST_USE_WINDOWS_H 1

#include "LogSink.h"
#include <fstream>
#include <string>
#include <mutex>

using std::string;

#define MEGA 0x100000
#define DEFAULT_MAX_SIZE MEGA * 50
#define DEFAULT_WINDOW_SIZE MEGA * 0x10

namespace qlog
{
	class FileSink : public LogSink
	{
	public:
		void handle_log_item(const LogItem* logItem) override;
		FileSink(const string& logFile);
		virtual ~FileSink(void);

		long max_log_file_size() const { return m_maxLogFileSize; }
		void set_max_log_file_size(long val) { m_maxLogFileSize = val; }
		size_t log_window_size() const { return m_logWindowSize; }
		void set_log_window_size(size_t val) { m_logWindowSize = val; }
		string log_file_name() const { return m_logFileName; }
		void set_log_file_name(const string& logFile);
	private:
		std::ofstream m_file;
		std::mutex m_fileMtx;
		string m_logFileName;

		long m_maxLogFileSize = DEFAULT_MAX_SIZE;
		size_t m_logWindowSize = DEFAULT_WINDOW_SIZE;

		void move_log_file_window();
	};
}
#endif
