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
		
	public:
		LogSink()
		{
			m_logT = std::thread([this]()
			{
				pool_slot<LogItem>* slot = nullptr;
				while (m_logItems.try_fetch_item(slot))
				{
					handle_log_item(slot->item());
					slot->dec_ref();
				}
			});
		}

		virtual ~LogSink()
		{
		}

		void submit(pool_slot<LogItem>* itemSlot)
		{
			itemSlot->inc_ref();
			m_logItems.put_item(itemSlot);
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
		ConsumerQ<pool_slot<LogItem>*> m_logItems;
		std::thread m_logT;
		
	};
}
#endif