#include "ViewportPanel.h"
namespace Laura
{
	void ViewportPanel::OnImGuiRender(std::shared_ptr<IImage2D> image, EditorState& editorState)
	{
		static ImGuiWindowFlags ViewportFlags = ImGuiWindowFlags_NoScrollbar | 
												ImGuiWindowFlags_NoTitleBar | 
												ImGuiWindowFlags_NoCollapse |
												ImGuiWindowFlags_MenuBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 }); // remove the border padding
		ImGui::Begin("ViewportPanel", nullptr, ViewportFlags);
		
		RenderSettingsMenu();

		if (image == nullptr)
		{
			ImGui::Text("No image to display");
			ImGui::PopStyleVar();
			ImGui::End();
			return;
		}
		
		ImageDimensions = image->GetDimensions();
		WindowDimensions = glm::ivec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		WindowPosition = glm::ivec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);

		bool DimensionsChanged = (ImageDimensions != m_PrevImageDimensions || WindowDimensions != m_PrevWindowDimensions);
		bool PositionChanged = (WindowPosition != m_PrevWindowPosition);
		
		if (editorState.viewportMode == ViewportMode::CENTERED)
		{
			glm::ivec2 offsetTopLeftCorner = (WindowDimensions - ImageDimensions) / 2;
			m_TopLeftImageCoords = offsetTopLeftCorner + WindowPosition;
			m_BottomRightImageCoords = offsetTopLeftCorner + WindowPosition + ImageDimensions;
		}

		if (editorState.viewportMode == ViewportMode::STRETCH_TO_VIEWPORT)
		{
			m_TopLeftImageCoords = WindowPosition;
			m_BottomRightImageCoords = WindowPosition + WindowDimensions;
		}

		if (editorState.viewportMode == ViewportMode::FIT_TO_VIEWPORT)
		{
			// if the ViewportPanel has been resized or the renderer output image size has been changed
			
			if (DimensionsChanged || PositionChanged)
			{
				if (DimensionsChanged)
				{
					m_PrevWindowDimensions = WindowDimensions;
					m_PrevImageDimensions = ImageDimensions;

					float WindowAspectRatio = (float)WindowDimensions.x / (float)WindowDimensions.y;
					float ImageAspectRatio = (float)ImageDimensions.x / (float)ImageDimensions.y;
					// if true width is the limiting factor (spans the entire width)
					if (WindowAspectRatio <= ImageAspectRatio)
					{
						m_TargetImageDimensions.x = WindowDimensions.x;
						m_TargetImageDimensions.y = ceil(WindowDimensions.x / ImageAspectRatio);
					}
					else // height is the limiting factor (spans the entire height)
					{
						m_TargetImageDimensions.x = ceil(WindowDimensions.y * ImageAspectRatio);
						m_TargetImageDimensions.y = WindowDimensions.y;
					}
				}

				m_PrevWindowPosition = WindowPosition;

				m_TopLeftImageCoords.x = (WindowDimensions.x - m_TargetImageDimensions.x) / 2.0f;
				m_TopLeftImageCoords.y = (WindowDimensions.y - m_TargetImageDimensions.y) / 2.0f;
				
				// offset by the viewport panel's position
				m_TopLeftImageCoords.x += WindowPosition.x;
				m_TopLeftImageCoords.y += WindowPosition.y;

				m_BottomRightImageCoords.x = m_TopLeftImageCoords.x + m_TargetImageDimensions.x;
				m_BottomRightImageCoords.y = m_TopLeftImageCoords.y + m_TargetImageDimensions.y;
			}
		}

		// ImGui Handles scaling up the texture			
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 TLImVec = ImVec2(m_TopLeftImageCoords.x, m_TopLeftImageCoords.y);
		ImVec2 BRImVec = ImVec2(m_BottomRightImageCoords.x, m_BottomRightImageCoords.y);
		drawList->AddImage((ImTextureID)image->GetID(), TLImVec, BRImVec, { 0, 1 }, { 1, 0 });

		ImGui::PopStyleVar();
		ImGui::End();
	}


	void Laura::ViewportPanel::RenderSettingsMenu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("settings"))
			{
			}
			ImGui::EndMenuBar();
		}
	}
}
