#include "ViewportPanel.h"
#include <IconsFontAwesome6.h>
#include <imgui_internal.h>

namespace Laura
{

	void ViewportPanel::OnImGuiRender(std::weak_ptr<IImage2D> image, std::shared_ptr<EditorState> editorState) {
		static ImGuiWindowFlags ViewportFlags = ImGuiWindowFlags_NoCollapse;
		
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 originalWindowBG = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_WindowBg] = style.Colors[ImGuiCol_TitleBg]; // make the background blend with the titlebar

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 }); // remove the border padding
		ImGui::Begin(ICON_FA_EYE " Viewport", nullptr, ViewportFlags);

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		
		ForceUpdate = false;

		DrawViewportSettingsPanel(editorState);

		auto imageShared = image.lock();
		if (imageShared == nullptr) {
			DrawVieportSettingsButton(editorState);
			ImGui::PopStyleVar();
			ImGui::End();
			return;
		}
		
		ImageDimensions = imageShared->GetDimensions();
		WindowDimensions = glm::ivec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		TLWindowPosition = glm::ivec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);

		bool DimensionsChanged = (ImageDimensions != m_PrevImageDimensions || WindowDimensions != m_PrevWindowDimensions);
		bool PositionChanged = (TLWindowPosition != m_PrevWindowPosition);

		if (editorState->persistent.viewportMode == ViewportMode::CenterToViewport) {
			if (DimensionsChanged || PositionChanged || ForceUpdate) {
				glm::ivec2 OffsetTopLeftCorner = (WindowDimensions - ImageDimensions) / 2;
				m_TopLeftImageCoords = OffsetTopLeftCorner + TLWindowPosition;
				m_BottomRightImageCoords = OffsetTopLeftCorner + TLWindowPosition + ImageDimensions;
			}
		}

		if (editorState->persistent.viewportMode == ViewportMode::StretchToViewport) {
			if (DimensionsChanged || PositionChanged || ForceUpdate) {
				m_TopLeftImageCoords = TLWindowPosition;
				m_BottomRightImageCoords = TLWindowPosition + WindowDimensions;
			}
		}

		if (editorState->persistent.viewportMode == ViewportMode::FitToViewport) {
			// if the ViewportPanel has been resized or the renderer output image size has been changed

			if (DimensionsChanged || PositionChanged || ForceUpdate) {
				if (DimensionsChanged || ForceUpdate) {
					m_PrevWindowDimensions = WindowDimensions;
					m_PrevImageDimensions = ImageDimensions;

					float WindowAspectRatio = (float)WindowDimensions.x / (float)WindowDimensions.y;
					float ImageAspectRatio = (float)ImageDimensions.x / (float)ImageDimensions.y;
					// if true width is the limiting factor (spans the entire width)
					if (WindowAspectRatio <= ImageAspectRatio) {
						m_TargetImageDimensions.x = WindowDimensions.x;
						m_TargetImageDimensions.y = ceil(WindowDimensions.x / ImageAspectRatio);
					}
					else { // height is the limiting factor (spans the entire height)
						m_TargetImageDimensions.x = ceil(WindowDimensions.y * ImageAspectRatio);
						m_TargetImageDimensions.y = WindowDimensions.y;
					}
				}

				m_PrevWindowPosition = TLWindowPosition;

				m_TopLeftImageCoords.x = (WindowDimensions.x - m_TargetImageDimensions.x) / 2.0f;
				m_TopLeftImageCoords.y = (WindowDimensions.y - m_TargetImageDimensions.y) / 2.0f;

				// offset by the viewport panel's position
				m_TopLeftImageCoords.x += TLWindowPosition.x;
				m_TopLeftImageCoords.y += TLWindowPosition.y;

				m_BottomRightImageCoords.x = m_TopLeftImageCoords.x + m_TargetImageDimensions.x;
				m_BottomRightImageCoords.y = m_TopLeftImageCoords.y + m_TargetImageDimensions.y;
			}
		}

		// ImGui Handles scaling up the texture			
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 TLImVec = ImVec2(m_TopLeftImageCoords.x, m_TopLeftImageCoords.y);
		ImVec2 BRImVec = ImVec2(m_BottomRightImageCoords.x, m_BottomRightImageCoords.y);
		drawList->AddImage((ImTextureID)imageShared->GetID(), TLImVec, BRImVec, { 0, 1 }, { 1, 0 });

		DrawVieportSettingsButton(editorState);

		ImGui::PopStyleVar();
		ImGui::End();
		style.Colors[ImGuiCol_WindowBg] = originalWindowBG;
	}

	void ViewportPanel::DrawVieportSettingsButton(std::shared_ptr<EditorState> editorState) {
		ImVec2 panelDims = ImGui::GetContentRegionAvail();
		float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImGui::Spacing();
		ImGui::SameLine(panelDims.x - lineHeight);
		if (ImGui::Button(ICON_FA_ELLIPSIS_VERTICAL, { lineHeight, lineHeight })) {
			editorState->temp.isViewportSettingsPanelOpen = true;
		}
	}

	void ViewportPanel::DrawViewportSettingsPanel(std::shared_ptr<EditorState> editorState) {
		if (!editorState->temp.isViewportSettingsPanelOpen) {
			return;
		}

		static ImGuiWindowFlags ViewportSettingsFlags = ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
		ImGui::Begin(ICON_FA_GEAR " Viewport Settings", &editorState->temp.isViewportSettingsPanelOpen, ViewportSettingsFlags);
		ImGui::Text("Viewport Mode");
		
		if (	ImGui::RadioButton("Center",	(int*)&editorState->persistent.viewportMode, (int)ViewportMode::CenterToViewport)  ||
				ImGui::RadioButton("Stretch",	(int*)&editorState->persistent.viewportMode, (int)ViewportMode::StretchToViewport) ||
				ImGui::RadioButton("Fit",		(int*)&editorState->persistent.viewportMode, (int)ViewportMode::FitToViewport)){
			ForceUpdate = true;
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}
}
