#include "Profiler.h"
#include "Profiler.h"

namespace Laura {

	void Profiler::startTimestamp(const std::string& id)
	{
		Timestamp ts;
		ts.start = std::chrono::high_resolution_clock::now();
		m_Timestamps.insert_or_assign(id, ts);
	}

	void Profiler::endTimestamp(const std::string& id)
	{
		auto it = m_Timestamps.find(id);
		if (it == m_Timestamps.end())
		{
			std::cout << "Invalid End Timestamp ID" << std::endl;
			return;
		}

		Timestamp& ts = it->second;
		ts.end = std::chrono::high_resolution_clock::now();
		ts.elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(ts.end - ts.start);
		ts.elapsed_ms = std::chrono::duration<double, std::milli>(ts.elapsed_ns).count();
	}

	const std::shared_ptr<const Timestamp> Profiler::getTimestamp(const std::string& id) const
	{
		auto it = m_Timestamps.find(id);
		if (it == m_Timestamps.end())
		{
			std::cout << "Invalid Get Timestamp ID" << std::endl;
			return nullptr;
		}
		return std::make_shared<const Timestamp>(it->second);
	}

	const std::unordered_map<std::string, Timestamp>& Laura::Profiler::getAllTimestamps() const
	{
		return m_Timestamps;
	}
}