#pragma once
#include "LogItem.h"
#include "Utils.hpp"
#include <vector>
#include "Formatter.hpp"

namespace qlog
{
	namespace formatters
	{
		class DefaultFormatter : Formatter
		{
		public:
			static std::string form_message(const LogItem& item)
			{				
				return format("%s (%d) %s %s [%s/%s/%d]\n", item.m_message.size(),
					util::time_point_to_string(item.m_logTime).c_str(),
					item.m_tid,
					log_level_to_string(item.m_level).c_str(),
					item.m_message.back() == '\n' ? std::string(item.m_message.begin(), item.m_message.end() - 1).c_str() : item.m_message.c_str(),
					item.m_function.c_str(),
					item.m_file.c_str(),
					item.m_line);				
			}
		};
	}
}
