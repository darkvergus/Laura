#pragma once

#include "lrpch.h"


namespace Laura
{
	struct Timestamp{
		std::chrono::high_resolution_clock::time_point start;
		std::chrono::high_resolution_clock::time_point end;
		std::chrono::nanoseconds elapsed_ns;
		double elapsed_ms;
		std::string id;
	};

	class Profiler
	{
	public:
		Profiler() = default;
		~Profiler() = default;

		void startTimestamp(const std::string& id);
		void endTimestamp(const std::string& id);

		const std::shared_ptr<const Timestamp> getTimestamp(const std::string& id);

	private:
		std::unordered_map<std::string, Timestamp> data;
	};
}