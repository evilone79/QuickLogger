#pragma once
#include "LogItem.h"
#include "Utils.hpp"
#include <vector>

namespace qlog
{
	namespace formatters
	{
		class DefaultFormatter
		{
		public:
			static std::string form_message(const LogItem& item)
			{
				auto format = "%s (%d) %s %s [%s/%s/%d]\n";
				std::vector<char> msg;
				msg.resize(item.m_message.size() + 256);
				std::sprintf(msg.data(), format,
					util::time_point_to_string(item.m_logTime).c_str(),
					item.m_tid,
					log_level_to_string(item.m_level).c_str(),
					item.m_message.c_str(),
					item.m_function.c_str(),
					item.m_file.c_str(),
					item.m_line);

				return std::string(std::begin(msg), std::end(msg));
			}
		};
	}
}
