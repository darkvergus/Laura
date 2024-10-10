#pragma once

#include "lrpch.h"

#include "spdlog/spdlog.h"

namespace Laura {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetEditorLogger() { return s_EditorLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_EditorLogger;
	};
}

#define LR_CORE_TRACE(...)    ::Laura::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LR_CORE_INFO(...)     ::Laura::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LR_CORE_WARN(...)     ::Laura::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LR_CORE_ERROR(...)    ::Laura::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LR_CORE_CRITICAL(...) ::Laura::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define LR_EDITOR_TRACE(...)    ::Laura::Log::GetEditorLogger()->trace(__VA_ARGS__)
#define LR_EDITOR_INFO(...)     ::Laura::Log::GetEditorLogger()->info(__VA_ARGS__)
#define LR_EDITOR_WARN(...)     ::Laura::Log::GetEditorLogger()->warn(__VA_ARGS__)
#define LR_EDITOR_ERROR(...)    ::Laura::Log::GetEditorLogger()->error(__VA_ARGS__)
#define LR_EDITOR_CRITICAL(...) ::Laura::Log::GetEditorLogger()->critical(__VA_ARGS__)
