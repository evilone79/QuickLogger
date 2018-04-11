// LoggerTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "QuickLogger.h"
#include "QuickLoggerSinks.h"
#include "QuickLoggerGlobal.h"
#include <thread>
#include <iostream>
#include <vector>

using namespace std;

struct Tester
{
	template<typename T>
	static void do_something(T a, std::string b)
	{
		std::cout << "just printing the msg " << b << " = " << a << "\n";
		LogDebug("received " << a << " and " << b);
	}
};

int main()
{
	QuickLog.set_log_level(qlog::QuickLogLevel::log_util);
	QuickLog.append_sink<qlog::sinks::DebugTraceSink, qlog::formatters::NoTimeFormatter>();
	QuickLog.append_sink<qlog::sinks::FileSink>().set_log_file_name("log_tester.log");
	QuickLog.append_sink<qlog::sinks::StdoutSink>();
	LogUtil("message..........");
	LogDebug("message..........");
	vector<thread> vec;
	for (int i=0; i<10; ++i)
	{
		vec.emplace_back([]{
			uint32_t cnt = 0;
			while (true)
			{
				auto start = std::chrono::system_clock::now();
				LogInfo("This message comes from thread no. " << this_thread::get_id);
				this_thread::sleep_for(std::chrono::milliseconds(1));
				auto took = std::chrono::system_clock::now() - start;				
				Tester::do_something(++cnt, std::to_string(cnt));
				//std::printf("loop took %lld\n", std::chrono::duration_cast<std::chrono::microseconds>(took).count());
				if (cnt % 10 == 0)
					LogError("This is an error message");
				if (cnt % 20 == 0)
					LogFatal("This is an fatal message");
			}
		});
	}
	for (auto& t : vec) t.join();
	std::cin.get();
    return 0;
}

