#include "LogManager.h"
#include "LogSink.h"

using namespace std;

qlog::LogManager::LogManager(void)
{	
}


qlog::LogManager::~LogManager(void)
{
}

void qlog::LogManager::flush()
{
	for (auto&& pSink : m_sinks) 
		pSink->synchronize();	
	m_pool.synchronize();
}

void qlog::LogManager::register_sink(std::unique_ptr<LogSink> sink)
{	
	m_sinks.push_back(std::move(sink));
}

qlog::pool_slot<qlog::LogItem>* qlog::LogManager::prepare_log_item()
{
	auto itemSlot = m_pool.get_slot();
	itemSlot->item()->m_logTime = sys_clk::now();
	itemSlot->item()->m_tid = std::this_thread::get_id();
	return itemSlot;
}

void qlog::LogManager::submit_log_item(pool_slot<LogItem>* logItem)
{
	for (auto&& sink : m_sinks)
	{
		sink->submit(logItem);
	}
}






