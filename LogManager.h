
#ifndef __InfraLogManager_H__
#define __InfraLogManager_H__
#define BOOST_USE_WINDOWS_H 1


#include "LogItem.h"
#include "ItemPool.hpp"

#include <memory>
#include <mutex>
#include <vector>

namespace qlog
{
	class LogSink;
	class LogManager
	{
	public:
		LogManager(void);
		virtual ~LogManager(void);
		void flush();

		void register_sink(std::unique_ptr<LogSink> sink);
		pool_slot<LogItem>* prepare_log_item();
		void submit_log_item(pool_slot<LogItem>* logItem);
	protected:

		std::vector<std::unique_ptr<LogSink>> m_sinks;
		sequential_pool<LogItem, 10000> m_pool;

	};
}
#endif //__InfraLogger_H__
