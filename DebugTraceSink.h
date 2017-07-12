#ifndef __DebugTraceSink_H__
#define __DebugTraceSink_H__
#define BOOST_USE_WINDOWS_H 1



#include "LogSink.h"
#include "FormatProvider.h"

namespace qlog
{
	namespace sinks
	{
		template<class Formatter>
		class DebugTraceSink : public LogSink, FormatProvider<Formatter>
		{
		public:
			DebugTraceSink() {}


			void handle_log_item(const LogItem* logItem) override;
		};

		template <class Formatter>
		void DebugTraceSink<Formatter>::handle_log_item(const LogItem* logItem)
		{
			auto msg = Formatter::form_message(*logItem);
			OutputDebugStringA(msg.c_str());
		}
	}
}
#endif