#pragma once

#include "Laura.h"

namespace Laura
{
	class ProfilerPanel
	{
	public:
		ProfilerPanel(std::shared_ptr<const Profiler> profiler);
		~ProfilerPanel() = default;

		void OnImGuiRender();
	private:
		std::shared_ptr<const Profiler> m_Profiler;

	};
}