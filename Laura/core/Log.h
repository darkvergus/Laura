#ifndef LOG_H
#define LOG_H

#include "lrpch.h"

#include "spdlog/spdlog.h"

class Log
{
public:
	Log();
	~Log();
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetAppLogger() { return s_AppLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_AppLogger;
};

#define CORE_TRACE(...)    ::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)     ::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)     ::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...)    ::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define APP_TRACE(...)    ::Log::GetAppLogger()->trace(__VA_ARGS__)
#define APP_INFO(...)     ::Log::GetAppLogger()->info(__VA_ARGS__)
#define APP_WARN(...)     ::Log::GetAppLogger()->warn(__VA_ARGS__)
#define APP_ERROR(...)    ::Log::GetAppLogger()->error(__VA_ARGS__)
#define APP_CRITICAL(...) ::Log::GetAppLogger()->critical(__VA_ARGS__)

#endif // LOG_H