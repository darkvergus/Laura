#pragma once 
#include "Laura.h"
#include "EditorState.h"
#include "EditorUI/Themes/EditorTheme.h"

namespace Laura
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(std::shared_ptr<EditorState> editorState, std::shared_ptr<ThemeManager> themeManager);
		~SceneHierarchyPanel() = default;

		void OnImGuiRender(std::shared_ptr<Scene> scene);
	private:
		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<ThemeManager> m_ThemeManager;
	};
}