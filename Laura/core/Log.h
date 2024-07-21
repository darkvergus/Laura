#ifndef LOG_H
#define LOG_H

#include "lrpch.h"

#include "spdlog/spdlog.h"

namespace Laura {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetAppLogger() { return s_AppLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_AppLogger;
	};
}

#define LR_CORE_TRACE(...)    ::Laura::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LR_CORE_INFO(...)     ::Laura::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LR_CORE_WARN(...)     ::Laura::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LR_CORE_ERROR(...)    ::Laura::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LR_CORE_CRITICAL(...) ::Laura::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define LR_APP_TRACE(...)    ::Laura::Log::GetAppLogger()->trace(__VA_ARGS__)
#define LR_APP_INFO(...)     ::Laura::Log::GetAppLogger()->info(__VA_ARGS__)
#define LR_APP_WARN(...)     ::Laura::Log::GetAppLogger()->warn(__VA_ARGS__)
#define LR_APP_ERROR(...)    ::Laura::Log::GetAppLogger()->error(__VA_ARGS__)
#define LR_APP_CRITICAL(...) ::Laura::Log::GetAppLogger()->critical(__VA_ARGS__)

#endif // LOG_H