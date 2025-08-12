#pragma once

#include "Laura.h"
#include "EditorState.h"

namespace Laura 
{

	class ViewportPanel {
	public:
		ViewportPanel(std::shared_ptr<EditorState> editorState, std::shared_ptr<ProjectManager> projectManager) 
			: m_EditorState(editorState), m_ProjectManager(projectManager) {}
		~ViewportPanel() = default;

		void OnImGuiRender(std::weak_ptr<IImage2D> image);
	
	private:
		void DrawDropTargetForScene();
		void DrawViewportSettingsPanel();
		void DrawVieportSettingsButton();
		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<ProjectManager> m_ProjectManager;

		glm::ivec2 m_TargetImageDimensions, m_PrevImageDimensions, m_PrevWindowDimensions;
		glm::ivec2 m_PrevWindowPosition, m_TopLeftImageCoords, m_BottomRightImageCoords;
		glm::ivec2 ImageDimensions, WindowDimensions, TLWindowPosition;
		bool ForceUpdate;
	};
}