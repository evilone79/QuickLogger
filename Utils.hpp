#pragma once


#include <sstream>
#include <chrono>
#include <iomanip>


using namespace std::chrono;

namespace qlog
{
	namespace util
	{
		template <typename Clock>
		static void time_point_to_string(const time_point<Clock>& tp, std::stringstream& ss)
		{
			auto time = system_clock::to_time_t(tp);
			auto timeStr = std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
			auto ms = duration_cast<milliseconds>(tp.time_since_epoch()) % 1000;
			ss << timeStr << "." << ms.count();
		}
	}
}

