#include "MainMenuPanel.h"

#include <imgui.h>
#include <IconsFontAwesome6.h>
#include "EditorUI/UtilityUI.h"

namespace Laura
{

	void Laura::MainMenuPanel::OnImGuiRender() {
		auto& theme = m_EditorState->temp.editorTheme;
		static bool shouldCloseProject = false;
		if (ImGui::BeginMainMenuBar()) {
			if (m_EditorState->temp.isInRuntimeMode) {
				ImGui::BeginDisabled();
			}
			float playBtnPos = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(ICON_FA_PLAY).x) / 2.0f;
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Save")) { m_ProjectManager->SaveProject(); }
				if (ImGui::MenuItem("Close")) { shouldCloseProject = true; }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window")) {
				bool themePanelDisabled = m_EditorState->temp.isThemePanelOpen;
				if (themePanelDisabled) { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem(ICON_FA_BRUSH " THEMES")) { m_EditorState->temp.isThemePanelOpen = true; }
				if (themePanelDisabled) { ImGui::EndDisabled(); }
				bool profilerPanelDisabled = m_EditorState->temp.isProfilerPanelOpen;
				if (profilerPanelDisabled) { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem(ICON_FA_STOPWATCH " PROFILER")) { m_EditorState->temp.isProfilerPanelOpen = true; }
				if (profilerPanelDisabled) { ImGui::EndDisabled(); }
				ImGui::EndMenu();
			}
			if (m_EditorState->temp.isInRuntimeMode) {
				ImGui::EndDisabled();
			}

			ImGui::SetCursorPosX(playBtnPos);
			EditorCol_ btnIconCol = (m_EditorState->temp.isInRuntimeMode) ? EditorCol_Warning : EditorCol_Text1;
			std::string btnIcon = (m_EditorState->temp.isInRuntimeMode) ? ICON_FA_SQUARE : ICON_FA_PLAY;
			theme.PushColor(ImGuiCol_Button, EditorCol_Transparent);
			theme.PushColor(ImGuiCol_Text, btnIconCol);
			if (ImGui::Button(btnIcon.c_str())) {
				m_EditorState->temp.isInRuntimeMode = !m_EditorState->temp.isInRuntimeMode;

				if (m_EditorState->temp.isInRuntimeMode) { // set runtime settings
					m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(m_ProjectManager->GetMutableRuntimeRenderSettings()));
				}
				else { // set editor settings
					m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(m_EditorState->persistent.editorRenderSettings));
				}
			}
			theme.PopColor(2);

			ImGui::EndMainMenuBar();
		}

		ConfirmWithCancel(
			shouldCloseProject,
			ICON_FA_DIAGRAM_PROJECT " Close Project",
			"Save before closing?",
			ICON_FA_FLOPPY_DISK " Save & Close Project",
			"Close Project",
			"Cancel",
			[&]() {
				m_ProjectManager->SaveProject();
				m_ProjectManager->CloseProject();
			},
			[&]() {
				m_ProjectManager->CloseProject();
			},
			m_EditorState
		);
	}
}
