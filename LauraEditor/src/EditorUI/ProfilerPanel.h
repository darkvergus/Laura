#pragma once

#include "Laura.h"

namespace Laura
{
	class ProfilerPanel
	{
	public:
		ProfilerPanel() = default;
		~ProfilerPanel() = default;

		void OnImGuiRender(std::shared_ptr<const Profiler> profiler);
	};
}