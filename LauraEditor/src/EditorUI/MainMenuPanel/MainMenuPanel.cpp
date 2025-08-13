#include "MainMenuPanel.h"

#include <imgui.h>
#include <IconsFontAwesome6.h>
#include "EditorUI/UtilityUI.h"

namespace Laura
{

	void Laura::MainMenuPanel::OnImGuiRender() {
		static bool shouldCloseProject = false;
		if (ImGui::BeginMainMenuBar()) {
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
