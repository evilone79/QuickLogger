#pragma once

#define QLOG_EXPORT _declspec(dllexport)

extern "C" QLOG_EXPORT void* g_QLoggerSingleton = nullptr;