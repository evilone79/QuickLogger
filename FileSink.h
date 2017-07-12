#ifndef __FileSink_H__
#define __FileSink_H__

#define BOOST_USE_WINDOWS_H 1

#include "LogSink.h"
#include <fstream>
#include <mutex>
#include "FormatProvider.h"
#include <filesystem>
#include <chrono>
#include "LogItem.h"


using std::string;

#define MEGA 0x100000
#define DEFAULT_MAX_SIZE MEGA * 50
#define DEFAULT_WINDOW_SIZE MEGA * 0x10

namespace qlog
{
	namespace sinks
	{
		template<class Formatter>
		class FileSink : public LogSink, FormatProvider<Formatter>
		{
		public:
			void handle_log_item(const LogItem* logItem) override;			
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


		namespace fs = std::experimental::filesystem;
		using namespace std::chrono;


		template<class Formatter>
		qlog::sinks::FileSink<Formatter>::~FileSink()
		{
			m_file.close();
		}

		template<class Formatter>
		void qlog::sinks::FileSink<Formatter>::handle_log_item(const LogItem* logItem)
		{			
			auto msg = Formatter::form_message(*logItem);
			std::lock_guard<std::mutex> lock(m_fileMtx);
			if (!m_file)
			{
				m_file.open(m_logFileName, std::ios::out | std::ios::app);
			}
			if (m_file)
			{
				m_file << msg;
				m_file << std::flush;
				if (m_file.tellp() >= m_maxLogFileSize)
				{
					move_log_file_window();
				}
			}
		}

		template<class Formatter>
		void qlog::sinks::FileSink<Formatter>::move_log_file_window()
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

		template<class Formatter>
		void qlog::sinks::FileSink<Formatter>::set_log_file_name(const string& val)
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

	}
}
#endif
