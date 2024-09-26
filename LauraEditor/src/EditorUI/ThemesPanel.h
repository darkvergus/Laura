#pragma once 
#include "Laura.h"
#include "EditorState.h"
#include "EditorTheme.h"

namespace Laura
{
	class ThemesPanel
	{
	public:
		ThemesPanel() = default;
		~ThemesPanel() = default;

		void OnImGuiRender(std::shared_ptr<EditorState> editorState, std::shared_ptr<ThemeManager> themeManager);
	};
}