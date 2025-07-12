#include <IconsFontAwesome6.h>
#include <imgui_internal.h>
#include <filesystem>
#include "ThemePanel.h"
#include "EditorUI/UtilityUI.h"

namespace Laura
{
	void ThemePanel::OnImGuiRender()
	{
		static std::string errorMessage = "";

		ImGuiWindowFlags ThemePanelFlags = ImGuiWindowFlags_NoDocking;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
		ImGui::Begin(ICON_FA_BRUSH " Themes", &m_EditorState->temp.ThemeSettingsPanelOpen, ThemePanelFlags);

		auto& theme = m_EditorState->temp.editorTheme;
		theme.PushColor(ImGuiCol_Button, theme.SECONDARY_2);

		std::filesystem::path themeFilePath(m_EditorState->persistent.editorThemeFilepath);
		std::string themeFileName = themeFilePath.filename().string();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Load Theme:");

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 60 - 4 - lineHeight); // 4 pixels padding between buttons

		if (ImGui::Button("Open..", ImVec2(60.0f, lineHeight))) {
			OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
			char buff[MAX_PATH] = {};
			ofn.lpstrFilter = EDITOR_THEME_FILE_EXTENSION "Files\0 * " EDITOR_THEME_FILE_EXTENSION "\0";
			ofn.lpstrTitle = "Select Theme:";
			ofn.nMaxFile = sizeof(buff);
			ofn.lpstrFile = buff;

			if (GetOpenFileNameA(&ofn)) { // when the user selected a file
				auto [success, errMsg] = theme.LoadFromFile(buff);
				if(success) {
					errorMessage = ""; // on success reset error message
					m_EditorState->persistent.editorThemeFilepath = buff;
					themeFilePath = std::filesystem::path(m_EditorState->persistent.editorThemeFilepath);
					themeFileName = themeFilePath.filename().string();
				}
				else {
					errorMessage = errMsg;
				}
			}
		}

		ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight);
		if(ImGui::Button(ICON_FA_ROTATE, ImVec2(lineHeight, lineHeight))) {
			// if exists a non default themepath
			if (m_EditorState->persistent.editorThemeFilepath != "") {
				auto [success, errMsg] = theme.LoadFromFile(m_EditorState->persistent.editorThemeFilepath);
				if (success) {
					errorMessage = "";
					themeFilePath = std::filesystem::path(m_EditorState->persistent.editorThemeFilepath);
					themeFileName = themeFilePath.filename().string();
				}
				else {
					errorMessage = errMsg;
				}
			}
		}

		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Reload the theme if you made changes to the .lrtheme file");
			ImGui::EndTooltip();
		}

		if (errorMessage != "") {
			ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), errorMessage.c_str(), ImGui::GetContentRegionAvail().x);
			ImGui::PopTextWrapPos();
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Restore Default Theme:");
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 70);

		static bool shouldSetDefaultTheme = false;
		if (ImGui::Button("Restore", ImVec2(70.0f, lineHeight))) {
			shouldSetDefaultTheme = true;
		}

		ConfirmAndExecute(shouldSetDefaultTheme, "Restore Default Theme", "Are you sure you want to restore the default theme?", [&]() {
				errorMessage = "";
				m_EditorState->persistent.editorThemeFilepath = "";
				theme.LoadDefaultDark();
			}, m_EditorState);

		ImGui::Dummy({ 0.0f, 20.0f });

		// Center Title
		float title_text_width = ImGui::CalcTextSize("Current Theme:").x;
		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - title_text_width) / 2.0f);
		ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f),"Current Theme:");

		// Center File Name
		std::string& file_text = theme.name;
		float file_text_width = ImGui::CalcTextSize(file_text.c_str()).x;
		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - file_text_width) / 2.0f);
		ImGui::Text(file_text.c_str());

		if (themeFileName != "" && ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone)) {
			ImGui::SetTooltip(m_EditorState->persistent.editorThemeFilepath.c_str());
		}

		ImGui::Dummy({ 0.0f, 20.0f });

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Create Template Theme:");
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 75.0f);
		static std::string saveErrorMessage = "";
		if (ImGui::Button(ICON_FA_PAINTBRUSH " Create", ImVec2(75.0f, lineHeight))) {
			OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
			char buff[MAX_PATH] = "template" EDITOR_THEME_FILE_EXTENSION;

			ofn.lpstrFilter = EDITOR_THEME_FILE_EXTENSION "Files\0 * " EDITOR_THEME_FILE_EXTENSION "\0";
			ofn.lpstrTitle = "Save Theme File";
			ofn.nMaxFile = sizeof(buff);
			ofn.lpstrFile = buff;
			ofn.Flags = OFN_OVERWRITEPROMPT;

			if (GetSaveFileNameA(&ofn)) {
				std::string filePath(buff);
				// append extension if missing
				if (!filePath.ends_with(EDITOR_THEME_FILE_EXTENSION)) { 
					filePath += std::string(EDITOR_THEME_FILE_EXTENSION);
				}

				auto [status, errMsg] = theme.SaveToFile(filePath);
				if (!status) {
					saveErrorMessage = errMsg;
				}
			}
		}

		if (saveErrorMessage != "") {
			ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), saveErrorMessage.c_str(), ImGui::GetContentRegionAvail().x);
			ImGui::PopTextWrapPos();
		}
		
		theme.PopColor();

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}
}