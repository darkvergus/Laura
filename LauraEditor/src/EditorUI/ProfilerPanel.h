#pragma once

#include "Laura.h"
#include "EditorState.h"
#include "EditorUI/Themes/EditorTheme.h"

namespace Laura
{

	class ProfilerPanel
	{
	public:
		ProfilerPanel(std::shared_ptr<EditorState> editorState, std::shared_ptr<ThemeManager> themeManager)
			: m_EditorState(editorState), m_ThemeManager(themeManager) {}

		~ProfilerPanel() = default;

		void OnImGuiRender(std::shared_ptr<Profiler> profiler);
	private:
		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<ThemeManager> m_ThemeManager;
	};
}