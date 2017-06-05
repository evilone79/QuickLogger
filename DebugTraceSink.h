#ifndef __DebugTraceSink_H__
#define __DebugTraceSink_H__
#define BOOST_USE_WINDOWS_H 1



#include "LogSink.h"

namespace qlog
{
	class DebugTraceSink : public LogSink
	{
	public:
		DebugTraceSink() {}


		void handle_log_item(const LogItem* logItem) override;
	};
}
#endif