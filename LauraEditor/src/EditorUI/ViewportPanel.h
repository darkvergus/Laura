#pragma once

#include "Laura.h"
#include "EditorState.h"

namespace Laura 
{

	class ViewportPanel
	{
	public:
		ViewportPanel() = default;
		~ViewportPanel() = default;

		void OnImGuiRender(std::shared_ptr<IImage2D> image, EditorState& editorState);
	
	private:
		void RenderSettingsMenu();

		glm::ivec2 m_TargetImageDimensions, m_PrevImageDimensions, m_PrevWindowDimensions;
		glm::ivec2 m_PrevWindowPosition, m_TopLeftImageCoords, m_BottomRightImageCoords;
		glm::ivec2 ImageDimensions, WindowDimensions, WindowPosition;
	};
}