#pragma once

#include <string>
#include <vector>


class Formatter
{
protected:
	template<typename ...Args>
	static std::string format(const char* format, size_t msgSize, Args&&... args)
	{		
		std::vector<char> msg ( msgSize + 256 );		
		auto written = std::sprintf(msg.data(), format, std::forward<Args&&>(args)...);
		return std::string(msg.data(), 0, written);
	}
};