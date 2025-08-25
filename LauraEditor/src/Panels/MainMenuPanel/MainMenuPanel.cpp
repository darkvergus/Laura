#include <imgui.h>
#include <IconsFontAwesome6.h>

#include "MainMenuPanel.h"
#include "Dialogs/ConfirmationDialog.h"
#include "Project/Scene/SceneManager.h"
#include "Dialogs/ProjectDialogs/ProjectDialogs.h"

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
		if (ImGui::BeginMainMenuBar()) {
			if (m_EditorState->temp.isInRuntimeSimulation) {
				ImGui::BeginDisabled();
			}
			float playBtnPos = (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(ICON_FA_PLAY).x) / 2.0f;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 3)); // no side padding, slight vertical padding	
			float orginalBorderSize = ImGui::GetStyle().PopupBorderSize;
			ImGui::GetStyle().PopupBorderSize = 0.0f;
			theme.PushColor(ImGuiCol_PopupBg, EditorCol_Background3);
			ImGui::SetNextWindowSizeConstraints(ImVec2(150, 0), ImVec2(FLT_MAX, FLT_MAX));
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem(ItemLabel(ICON_FA_FILE_CIRCLE_PLUS, "New").c_str(), "Ctrl+N")) { m_EditorState->temp.isCreateProjectDialogOpen = true; }
				if (ImGui::MenuItem(ItemLabel(ICON_FA_FOLDER_CLOSED, "Open...").c_str(), "Ctrl+O")) { m_ShouldOpenProject = true; }
				ImGui::Separator();
				if (ImGui::MenuItem(ItemLabel(ICON_FA_FLOPPY_DISK, "Save").c_str(), "Ctrl+S")) { m_ProjectManager->SaveProject(); }
				ImGui::MenuItem(ItemLabel("", "Save As...").c_str());
				ImGui::Separator();
				if (ImGui::MenuItem(ItemLabel(ICON_FA_ARROW_UP_FROM_BRACKET, "Export...").c_str())) { m_EditorState->temp.shouldOpenExportPanel = true; }
				ImGui::Separator();
				if (ImGui::MenuItem(ItemLabel("", "Close").c_str())) { m_ShouldCloseProject = true; }
				ImGui::EndMenu();
			}

			ImGui::SetNextWindowSizeConstraints(ImVec2(150, 0), ImVec2(FLT_MAX, FLT_MAX));
			if (ImGui::BeginMenu("View")) {
				if (ImGui::MenuItem(ItemLabel(ICON_FA_STOPWATCH, "Profiler").c_str(), NULL, false, !m_EditorState->temp.isProfilerPanelOpen)) {
					m_EditorState->temp.isProfilerPanelOpen = true; 
				}
				ImGui::Separator();
				if (ImGui::MenuItem(ItemLabel("", "Themes").c_str(), NULL, false, !m_EditorState->temp.isThemePanelOpen)) {
					m_EditorState->temp.isThemePanelOpen = true; 
				}
				ImGui::EndMenu();
			}

			ImGui::GetStyle().PopupBorderSize = orginalBorderSize;
			theme.PopColor();
			ImGui::PopStyleVar();

			if (m_EditorState->temp.isInRuntimeSimulation) {
				ImGui::EndDisabled();
			}

			if (m_ProjectManager->ProjectIsOpen()) {
				ImGui::SetCursorPosX(playBtnPos);
				EditorCol_ btnIconCol = (m_EditorState->temp.isInRuntimeSimulation) ? EditorCol_Warning : EditorCol_Text1;
				std::string btnIcon = (m_EditorState->temp.isInRuntimeSimulation) ? ICON_FA_SQUARE : ICON_FA_PLAY;
				theme.PushColor(ImGuiCol_Button, EditorCol_Transparent, 0.0f);
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
		
		// only visible if flag is true (flags are set in the menu bar)
		CreateProjectDialog(m_ProjectManager, m_EditorState);

		ConfirmWithCancel(
			m_ShouldOpenProject, 
			ICON_FA_FOLDER_CLOSED " Open", 
			"Save current project before opening?", 
			ICON_FA_FLOPPY_DISK " Save & Open",
			"Open",
			"Cancel",
			[&]() {
				m_ProjectManager->SaveProject();
			}, 
			[&](){},
			m_EditorState,
			[&]() {
				OpenProjectDialog(m_ProjectManager);
			}
		);

		ConfirmWithCancel(
			m_ShouldCloseProject,
			ICON_FA_DIAGRAM_PROJECT " Close Project",
			"Save project before closing?",
			ICON_FA_FLOPPY_DISK " Save & Close",
			"Close",
			"Cancel",
			[&]() {
				m_ProjectManager->SaveProject();
			},
			[&](){},
			m_EditorState,
			[&]() {
				m_ProjectManager->CloseProject();
			}
		);
	}
 }
