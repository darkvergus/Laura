#include "ViewportPanel.h"
#include <IconsFontAwesome6.h>
#include <imgui_internal.h>
#include "EditorUI/DNDPayloads.h"
#include "EditorUI/UtilityUI.h"

namespace Laura
{
	void ViewportPanel::DrawDropTargetForScene() {
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DNDPayloadTypes::SCENE)) {
				IM_ASSERT(payload->DataSize == sizeof(DNDPayload));
				auto& scenePayload = *static_cast<DNDPayload*>(payload->Data);
				if (m_ProjectManager->ProjectIsOpen()) {
					if (auto sceneManager = m_ProjectManager->GetSceneManager()) {
						sceneManager->SetOpenScene(scenePayload.guid);
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	void ViewportPanel::onEvent(std::shared_ptr<IEvent> event) {
		if (event->GetType() == EventType::NEW_FRAME_RENDERED_EVENT) {
			m_LatestRenderedFrame = std::dynamic_pointer_cast<NewFrameRenderedEvent>(event)->frame;
		}
	}

	void ViewportPanel::OnImGuiRender() {
		static ImGuiWindowFlags ViewportFlags = ImGuiWindowFlags_NoCollapse;
		auto theme = m_EditorState->temp.editorTheme;


		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 originalWindowBG = style.Colors[ImGuiCol_WindowBg];
		theme.PushColor(ImGuiCol_WindowBg, EditorCol_Background2);
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 }); // remove the border padding
		ImGui::Begin(ICON_FA_EYE " VIEWPORT", nullptr, ViewportFlags);
		if (m_EditorState->temp.isInRuntimeMode) {
			ImGui::BeginDisabled();
		}

		ImGui::BeginChild("DropArea");
	
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ForceUpdate = false;

		DrawViewportSettingsPanel();

		auto latestRenderedFrameShared = m_LatestRenderedFrame.lock();
		if (latestRenderedFrameShared == nullptr) {
			DrawVieportSettingsButton();
			ImGui::EndChild();
			ImGui::PopStyleVar();
			DrawDropTargetForScene();
			
			if (m_EditorState->temp.isInRuntimeMode) {
				ImGui::EndDisabled();
			}
			
			ImGui::End();
			theme.PopColor();
			return;
		}
		
		ImageDimensions = latestRenderedFrameShared->GetDimensions();
		WindowDimensions = glm::ivec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		TLWindowPosition = glm::ivec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);

		bool DimensionsChanged = (ImageDimensions != m_PrevImageDimensions || WindowDimensions != m_PrevWindowDimensions);
		bool PositionChanged = (TLWindowPosition != m_PrevWindowPosition);

		if (m_EditorState->persistent.viewportMode == ViewportMode::CenterToViewport) {
			if (DimensionsChanged || PositionChanged || ForceUpdate) {
				glm::ivec2 OffsetTopLeftCorner = (WindowDimensions - ImageDimensions) / 2;
				m_TopLeftImageCoords = OffsetTopLeftCorner + TLWindowPosition;
				m_BottomRightImageCoords = OffsetTopLeftCorner + TLWindowPosition + ImageDimensions;
			}
		}

		if (m_EditorState->persistent.viewportMode == ViewportMode::StretchToViewport) {
			if (DimensionsChanged || PositionChanged || ForceUpdate) {
				m_TopLeftImageCoords = TLWindowPosition;
				m_BottomRightImageCoords = TLWindowPosition + WindowDimensions;
			}
		}

		if (m_EditorState->persistent.viewportMode == ViewportMode::FitToViewport) {
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
		drawList->AddImage((ImTextureID)latestRenderedFrameShared->GetID(), TLImVec, BRImVec, { 0, 1 }, { 1, 0 });

		DrawVieportSettingsButton();

		ImGui::EndChild();
		ImGui::PopStyleVar();
		DrawDropTargetForScene();

		if (m_EditorState->temp.isInRuntimeMode) {
			ImGui::EndDisabled();
		}

		ImGui::End();
		theme.PopColor();
	}

	void ViewportPanel::DrawVieportSettingsButton() {
		auto theme = m_EditorState->temp.editorTheme;
		ImVec2 panelDims = ImGui::GetContentRegionAvail();
		float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImGui::Spacing();
		ImGui::SameLine(panelDims.x - lineHeight);
		theme.PushColor(ImGuiCol_Button, EditorCol_Background3);
		if (ImGui::Button(ICON_FA_ELLIPSIS_VERTICAL, { lineHeight, lineHeight })) {
			m_EditorState->temp.isViewportSettingsPanelOpen = true;
		}
		theme.PopColor(); // button
	}

	void ViewportPanel::DrawViewportSettingsPanel() {
		auto theme = m_EditorState->temp.editorTheme;
		if (!m_EditorState->temp.isViewportSettingsPanelOpen) {
			return;
		}

		static ImGuiWindowFlags ViewportSettingsFlags = ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoCollapse;
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
		theme.PushColor(ImGuiCol_WindowBg, EditorCol_Background3);
		ImGui::SetNextWindowSizeConstraints({ 250.0f, 150.0f }, { FLT_MAX, FLT_MAX });
		ImGui::Begin(ICON_FA_EYE " VIEWPORT OPTIONS", &m_EditorState->temp.isViewportSettingsPanelOpen, ViewportSettingsFlags);
		
		ViewportMode& mode = m_EditorState->persistent.viewportMode;
		int currentMode = static_cast<int>(mode);
		theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
		ImGui::Text("Display: ");
		theme.PopColor();
		ImGui::SameLine();

		theme.PushColor(ImGuiCol_FrameBg, EditorCol_Background1);
			if (ImGui::Combo("##Viewport Mode", &currentMode, ViewportModeStr, IM_ARRAYSIZE(ViewportModeStr))) {
				mode = static_cast<ViewportMode>(currentMode);
				ForceUpdate = true;
			}
		theme.PopColor();

		ImGui::End();
		theme.PopColor();
		ImGui::PopStyleVar();
	}
}
