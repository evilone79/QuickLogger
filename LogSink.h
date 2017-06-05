#ifndef __ILogSink_H__
#define __ILogSink_H__


#include <memory>
#include "LogItem.h"
#include "ConsumerQ.hpp"
#include "LogManager.h"

namespace qlog
{
	class LogSink
	{
		friend class LogManager;
	public:
		LogSink()
		{
			m_logT = std::thread([this]()
			{
				LogItem* item = nullptr;
				while (m_logItems.try_fetch_item(item))
				{
					handle_log_item(item);
					m_logManager->release_log_item(item);
				}
			});
		}

		virtual ~LogSink()
		{
		}

		void submit(LogItem* logItem)
		{
			m_logItems.put_item(logItem);
		}

		void synchronize()
		{
			m_logItems.initiate_teardown();
			if (m_logT.joinable())
			{
				m_logT.join();
			}
		}
	protected:
		virtual void handle_log_item(const LogItem* item) = 0;
		LogManager* m_logManager;
		ConsumerQ<LogItem*> m_logItems;
		std::thread m_logT;

	};
}
#endif