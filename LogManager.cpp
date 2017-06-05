#include "LogManager.h"
#include "LogSink.h"

using namespace std;

qlog::LogManager::LogManager(void)
{	
}


qlog::LogManager::~LogManager(void)
{
	for (auto&& pSink : m_sinks) 
		pSink->synchronize();	
	m_pool.synchronize();
}

void qlog::LogManager::register_sink(std::unique_ptr<LogSink> sink)
{
	sink->m_logManager = this;
	m_sinks.push_back(std::move(sink));
}

qlog::LogItem* qlog::LogManager::prepare_log_item()
{
	auto logItem = m_pool.get_item();
	logItem->m_logTime = sys_clk::now();
	logItem->m_tid = std::this_thread::get_id();
	return logItem;
}

void qlog::LogManager::submit_log_item(LogItem* logItem)
{
	for (auto&& sink : m_sinks)
	{
		sink->submit(logItem);
	}
}

void qlog::LogManager::release_log_item(LogItem* logItem)
{
	++logItem->m_releaseCount;
	auto sinkCnt = m_sinks.size();
	if (logItem->m_releaseCount.compare_exchange_strong(sinkCnt, size_t{ 0 }))
	{		
		m_pool.release_item(logItem);
	}
}






