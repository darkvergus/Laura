#pragma once

#include "Laura.h"
#include "EditorUI/ThemePanel/ThemePanel.h"
#include "EditorState.h"

namespace Laura
{

	/*
		Executes the OnConfirm function. If "doubleConfirmEnabled" is enabled in the editor state, 
		a popup will prompt the user for confirmation first. To use this with a button click, 
		set a bool to true and pass it as the "execute" parameter. This bool ensures the popup 
		shows, as calling this function directly in the button's if clause won't trigger it.
	*/
	template <typename OnConfirm>
	void ConfirmAndExecute(	bool& shouldExecute, 
							const char* popupTitle,		
							const char* popupMessage, 
							OnConfirm onConfirm, 
							std::shared_ptr<EditorState> editorState) {
		if (!shouldExecute) {
			return;
		}
		
		// early return if double confirmation not needed
		if (!editorState->persistent.doubleConfirmEnabled) {
			onConfirm();
			shouldExecute = false;
			return;
		}
		
		EditorTheme& theme = editorState->temp.editorTheme;

		ImGui::OpenPopup(popupTitle);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

		if (ImGui::BeginPopupModal(popupTitle, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(popupMessage);
			float panelWidth = ImGui::GetContentRegionAvail().x;
			float buttonWidth = panelWidth * 0.5f - 5.0f;
			ImGui::Dummy({ 5.0f, 0.0f });

			theme.PushColor(ImGuiCol_Button, EditorCol_Secondary2);
			{
				if (ImGui::Button("Yes", ImVec2(buttonWidth, 0))) {
					onConfirm();
					shouldExecute = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();
				if (ImGui::Button("No", ImVec2(buttonWidth, 0))) {
					shouldExecute = false;
					ImGui::CloseCurrentPopup();
				}
			}
			theme.PopColor();
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
	}
}