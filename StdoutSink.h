#pragma once
#include "LogSink.h"
#include <windows.h>
#include "FormatProvider.h"


#define CON_BACK_BLACK      0

#define CON_FORE_BLACK      0
#define CON_FORE_RED       FOREGROUND_RED
#define CON_FORE_LIGHT_RED    FOREGROUND_RED | FOREGROUND_INTENSITY
#define CON_FORE_GREEN      FOREGROUND_GREEN
#define CON_FORE_LIGHT_GREEN   FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define CON_FORE_YELLOW      FOREGROUND_GREEN | FOREGROUND_RED
#define CON_FORE_LIGHT_YELLOW   FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define CON_FORE_GRAY       FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
#define CON_FORE_WHITE      FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY

namespace qlog
{
	namespace sinks
	{
		template<class Formatter>
		class StdoutSink : public LogSink, FormatProvider<Formatter>
		{
		public:
			StdoutSink();
			~StdoutSink();

		protected:
			void handle_log_item(const LogItem* item) override;

		private:
			QuickLogLevel m_lastLevel;
			CONSOLE_SCREEN_BUFFER_INFO m_lastConAttributes;
			HANDLE m_hStdout = nullptr;

			void set_font_color(QuickLogLevel logLevel) const;
		};

		template <class Formatter>
		qlog::sinks::StdoutSink<Formatter>::StdoutSink()
		{
			m_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			set_font_color(QuickLogLevel::log_debug);
			GetConsoleScreenBufferInfo(m_hStdout, &m_lastConAttributes);
		}

		template <class Formatter>
		qlog::sinks::StdoutSink<Formatter>::~StdoutSink()
		{
			SetConsoleTextAttribute(m_hStdout, m_lastConAttributes.wAttributes);
			FlushConsoleInputBuffer(m_hStdout);
		}

		template<class Formatter>
		void qlog::sinks::StdoutSink<Formatter>::handle_log_item(const LogItem * item)
		{
			if (m_lastLevel != item->m_level)
			{
				set_font_color(item->m_level);
			}
			
			auto msg = Formatter::form_message(*item);
			std::printf(msg.c_str());
		}

		template<class Formatter>
		void qlog::sinks::StdoutSink<Formatter>::set_font_color(QuickLogLevel logLevel) const
		{
			WORD colorSet = CON_BACK_BLACK | CON_FORE_GRAY;
			switch (logLevel)
			{
			case QuickLogLevel::log_critical:
			case QuickLogLevel::log_error:
				colorSet = CON_BACK_BLACK | CON_FORE_LIGHT_RED;
				break;
			case QuickLogLevel::log_warning:
				colorSet = CON_BACK_BLACK | CON_FORE_LIGHT_YELLOW;
				break;
			case QuickLogLevel::log_info:
				colorSet = CON_BACK_BLACK | CON_FORE_LIGHT_GREEN;
				break;
			case QuickLogLevel::log_debug:
			case QuickLogLevel::log_none:
				colorSet = CON_BACK_BLACK | CON_FORE_GRAY;
				break;
			}

			SetConsoleTextAttribute(m_hStdout, colorSet);
		}

	}

}


