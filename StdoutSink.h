#pragma once
#include "LogSink.h"
#include <windows.h>
#include "FormatProvider.h"


//#define CON_BACK_BLACK			0
//
//#define CON_FORE_BLACK			0
//#define CON_FORE_RED			FOREGROUND_RED
//#define CON_FORE_LIGHT_RED		FOREGROUND_RED | FOREGROUND_INTENSITY
//#define CON_FORE_GREEN			FOREGROUND_GREEN
//#define CON_FORE_LIGHT_GREEN	FOREGROUND_GREEN | FOREGROUND_INTENSITY
//#define CON_FORE_YELLOW			FOREGROUND_GREEN | FOREGROUND_RED
//#define CON_FORE_LIGHT_YELLOW   FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
//#define CON_FORE_GRAY			FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
//#define CON_FORE_WHITE			FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY

namespace qlog
{
	namespace sinks
	{
		template<class Formatter>
		class StdoutSink : public LogSink, FormatProvider<Formatter>
		{
			enum {
				BLACK = 0,
				DARKBLUE = FOREGROUND_BLUE,
				DARKGREEN = FOREGROUND_GREEN,
				DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
				DARKRED = FOREGROUND_RED,
				DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
				DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
				DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
				GRAY = FOREGROUND_INTENSITY,
				BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
				GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
				CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
				RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
				MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
				YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,				
				WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
			};
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
			WORD colorSet = BLACK | DARKGRAY;
			switch (logLevel)
			{
			case QuickLogLevel::log_fatal:
				colorSet = BLACK | MAGENTA;
				break;
			case QuickLogLevel::log_error:
				colorSet = BLACK | RED;
				break;
			case QuickLogLevel::log_warning:
				colorSet = BLACK | YELLOW;
				break;
			case QuickLogLevel::log_info:
				colorSet = BLACK | GREEN;
				break;
			case QuickLogLevel::log_debug:
				colorSet = BLACK | GRAY;
				break;
			case QuickLogLevel::log_util:
				colorSet = BLACK | DARKGRAY;
				break;
			}

			SetConsoleTextAttribute(m_hStdout, colorSet);
		}

	}

}


