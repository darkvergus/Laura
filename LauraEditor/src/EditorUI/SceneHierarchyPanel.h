#pragma once 
#include "Laura.h"
#include "EditorState.h"

namespace Laura
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		~SceneHierarchyPanel() = default;

		void OnImGuiRender(std::shared_ptr<Scene> scene, std::shared_ptr<EditorState> editorState);
	};
}