#pragma once 

#include "Laura.h"
#include "EditorState.h"

namespace Laura
{

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel(std::shared_ptr<EditorState> editorState, std::shared_ptr<ProjectManager> projectManager);
		~SceneHierarchyPanel() = default;
		void OnImGuiRender();

	private:
		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<ProjectManager> m_ProjectManager;
	};
}