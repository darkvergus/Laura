#pragma once
#include <filesystem>

namespace Laura
{
	namespace RuntimeCfg
	{
		// Defined in the .cpp
		extern std::filesystem::path EXECUTABLE_DIR;

		inline void Init(const std::filesystem::path& exeDir) {
			EXECUTABLE_DIR = exeDir;
		}
	}
}