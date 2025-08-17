#include "ExportPanel.h"
#include <IconsFontAwesome6.h>
namespace Laura
{

	void Laura::ExportPanel::OnImGuiRender() {
		EditorTheme& theme = m_EditorState->temp.editorTheme;

        if (!m_EditorState->temp.isExportPanelOpen) {
            return;
        }

        theme.PushColor(ImGuiCol_WindowBg, EditorCol_Background3);
        ImGui::Begin(ICON_FA_ARROW_UP_FROM_BRACKET " Export", &m_EditorState->temp.isProfilerPanelOpen); 
        if (m_EditorState->temp.isInRuntimeSimulation) {
            ImGui::BeginDisabled();
        }

        ImGui::Text("Hello");


        if (m_EditorState->temp.isInRuntimeSimulation) {
            ImGui::EndDisabled();
        }
        ImGui::End();
        theme.PopColor(); // windowBg
	}
}
