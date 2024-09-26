#pragma once 
#include "Laura.h"
#include "EditorState.h"
#include "EditorTheme.h"

namespace Laura
{
	class InspectorPanel
	{
	public:
		InspectorPanel(std::shared_ptr<EditorState> editorState, std::shared_ptr<ThemeManager> themeManager);
		~InspectorPanel() = default;

		void OnImGuiRender(std::shared_ptr<Scene> scene);
	private:
		template<typename T, typename UILambda>
		void DrawComponent(const std::string& TreenodeTitle, Entity entity, UILambda uiLambda, bool removable = true);

		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<ThemeManager> m_ThemeManager;
	};

}