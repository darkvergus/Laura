#pragma once
#include "Laura.h"
#include "EditorUI/Themes/EditorTheme.h"
#include "EditorState.h"

namespace Laura
{
	// Executes the OnConfirm function. If "doubleConfirmation" is enabled in the editor state, 
	// a popup will prompt the user for confirmation first. To use this with a button click, 
	// set a bool to true and pass it as the "execute" parameter. This bool ensures the popup 
	// shows, as calling this function directly in the button's if clause won't trigger it.
	template <typename OnConfirm>
	void ConfirmAndExecute(bool& execute, const char* popupTitle, const char* popupMessage, OnConfirm onConfirm, std::shared_ptr<ThemeManager> themeManager, std::shared_ptr<EditorState> editorState)
	{
		if (!execute)
		{
			return;
		}

		if (!editorState->persistent.doubleConfirmation)
		{
			onConfirm();
			execute = false;
			return;
		}

		ImGui::OpenPopup(popupTitle);
		std::cout << "Opening popup" << std::endl;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
		if (ImGui::BeginPopupModal(popupTitle, NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			std::cout << "Popup open" << std::endl;
			ImGui::Text(popupMessage);
			float panelWidth = ImGui::GetContentRegionAvail().x;
			float buttonWidth = panelWidth * 0.5f - 5.0f;
			ImGui::Dummy({ 5.0f, 0.0f });

			themeManager->ImGuiSet(ImGuiCol_Button, themeManager->GetActiveTheme()->ButtonGray);
			if (ImGui::Button("Yes", ImVec2(buttonWidth, 0)))
			{
				onConfirm();
				execute = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(buttonWidth, 0)))
			{
				execute = false;
				ImGui::CloseCurrentPopup();
			}
			themeManager->ImGuiSet(ImGuiCol_Button, themeManager->GetActiveTheme()->DefaultButton);
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
	}
}