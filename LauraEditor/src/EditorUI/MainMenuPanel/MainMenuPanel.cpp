#include "MainMenuPanel.h"

#include <imgui.h>
#include <IconsFontAwesome6.h>
#include "EditorUI/UtilityUI.h"

namespace Laura
{

	void MainMenuPanel::OnImGuiRender() {
		auto ItemLabel = [](const std::string& icon, const std::string& label) -> std::string {
			if (icon.empty()) {
				return "         " + label; // 9 spaces before label
			}
			return "   " + icon + "  " + label; // 3 spaces + icon + 2 spaces + label
		};

		auto& theme = m_EditorState->temp.editorTheme;
		static bool shouldCloseProject = false;
		if (ImGui::BeginMainMenuBar()) {
			if (m_EditorState->temp.isInRuntimeSimulation) {
				ImGui::BeginDisabled();
			}
			float playBtnPos = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(ICON_FA_PLAY).x) / 2.0f;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 3)); // no side padding, slight vertical padding	
			float orginalBorderSize = ImGui::GetStyle().PopupBorderSize;
			ImGui::GetStyle().PopupBorderSize = 0.0f;
			theme.PushColor(ImGuiCol_PopupBg, EditorCol_Background3, 0.9f);
			if (ImGui::BeginMenu("File")) {
				ImGui::MenuItem(ItemLabel(ICON_FA_FILE_CIRCLE_PLUS, "New").c_str(), "Ctrl+N");
				ImGui::MenuItem(ItemLabel(ICON_FA_FOLDER_CLOSED, "Open...").c_str(), "Ctrl+O");
				ImGui::Separator();
				ImGui::MenuItem(ItemLabel(ICON_FA_FLOPPY_DISK, "Save").c_str(), "Ctrl+S");
				ImGui::MenuItem(ItemLabel("", "Save As...").c_str());
				ImGui::Separator();
				ImGui::MenuItem(ItemLabel(ICON_FA_ARROW_UP_FROM_BRACKET, "Export...").c_str());
				ImGui::Separator();
				ImGui::MenuItem(ItemLabel("", "Close").c_str());
				ImGui::EndMenu();
			}
			ImGui::GetStyle().PopupBorderSize = orginalBorderSize;
			theme.PopColor();
			ImGui::PopStyleVar();

			if (ImGui::BeginMenu("Window")) {
				bool themePanelDisabled = m_EditorState->temp.isThemePanelOpen;
				if (themePanelDisabled) { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem(ICON_FA_BRUSH " Themes")) { m_EditorState->temp.isThemePanelOpen = true; }
				if (themePanelDisabled) { ImGui::EndDisabled(); }
				bool profilerPanelDisabled = m_EditorState->temp.isProfilerPanelOpen;
				if (profilerPanelDisabled) { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem(ICON_FA_STOPWATCH " Profiler")) { m_EditorState->temp.isProfilerPanelOpen = true; }
				if (profilerPanelDisabled) { ImGui::EndDisabled(); }
				ImGui::EndMenu();
			}
			if (m_EditorState->temp.isInRuntimeSimulation) {
				ImGui::EndDisabled();
			}

			if (m_ProjectManager->ProjectIsOpen()) {
				ImGui::SetCursorPosX(playBtnPos);
				EditorCol_ btnIconCol = (m_EditorState->temp.isInRuntimeSimulation) ? EditorCol_Warning : EditorCol_Text1;
				std::string btnIcon = (m_EditorState->temp.isInRuntimeSimulation) ? ICON_FA_SQUARE : ICON_FA_PLAY;
				theme.PushColor(ImGuiCol_Button, EditorCol_Transparent);
				theme.PushColor(ImGuiCol_Text, btnIconCol);
				if (ImGui::Button(btnIcon.c_str())) {
					m_EditorState->temp.isInRuntimeSimulation = !m_EditorState->temp.isInRuntimeSimulation;

					if (m_EditorState->temp.isInRuntimeSimulation) { // set runtime settings
						m_ProjectManager->GetSceneManager()->EnterRuntimeSimulation();
							m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(m_ProjectManager->GetMutableRuntimeRenderSettings()));
					}
					else { // set editor settings
						m_ProjectManager->GetSceneManager()->ExitRuntimeSimulation();
						m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(m_EditorState->persistent.editorRenderSettings));
					}
				}
				theme.PopColor(2);
			}

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
