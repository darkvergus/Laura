#pragma once 
#include "Laura.h"
#include "EditorState.h"

namespace Laura
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(std::shared_ptr<EditorState> editorState);
		~SceneHierarchyPanel() = default;

		void OnImGuiRender(std::shared_ptr<Scene> scene);
	private:
		std::shared_ptr<EditorState> m_EditorState;
	};
}