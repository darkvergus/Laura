#pragma once

#include "Laura.h"
#include "EditorState.h"
//#include "IconsFontAwesome6.h"

namespace Laura 
{

	class ViewportPanel
	{
	public:
		ViewportPanel() = default;
		~ViewportPanel() = default;

		void OnImGuiRender(std::shared_ptr<IImage2D> image, std::shared_ptr<EditorState> editorState);
	
	private:
		void DrawViewportSettingsPanel(std::shared_ptr<EditorState> editorState);
		glm::ivec2 m_TargetImageDimensions, m_PrevImageDimensions, m_PrevWindowDimensions;
		glm::ivec2 m_PrevWindowPosition, m_TopLeftImageCoords, m_BottomRightImageCoords;
		glm::ivec2 ImageDimensions, WindowDimensions, TLWindowPosition;
		bool ForceUpdate;
	};
}