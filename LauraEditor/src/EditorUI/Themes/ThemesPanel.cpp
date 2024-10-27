#include <IconsFontAwesome6.h>
#include <imgui_internal.h>
#include <filesystem>
#include "ThemesPanel.h"
#include "EditorUI/UtilityUI.h"

namespace Laura
{
	void ThemesPanel::OnImGuiRender()
	{
		ImGuiWindowFlags ThemesPanelFlags = ImGuiWindowFlags_NoDocking;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
		ImGui::Begin(ICON_FA_BRUSH " Themes", &m_EditorState->temp.ThemeSettingsPanelOpen, ThemesPanelFlags);

		m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->ButtonGray);

		std::filesystem::path themeFilePath(m_EditorState->persistent.ThemeFilePath);
		std::string themeFileName = themeFilePath.filename().string();
		static std::string loadThemeStatusMessage = "";

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Load Theme:");
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 60 - 4 - lineHeight); // 4 pixels padding between buttons
		if (ImGui::Button("Open..", ImVec2(60.0f, lineHeight)))
		{
			OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
			char buff[MAX_PATH] = {};
			ofn.lpstrFilter = LR_THEME_FILE_EXTENSION "Files\0 * " LR_THEME_FILE_EXTENSION "\0";
			ofn.lpstrTitle = "Select Theme:";
			ofn.nMaxFile = sizeof(buff);
			ofn.lpstrFile = buff;
			if (GetOpenFileNameA(&ofn)) // if the user selected a file
			{
				if(m_ThemeManager->LoadTheme(buff, loadThemeStatusMessage))
				{
					loadThemeStatusMessage = "";
					m_EditorState->persistent.ThemeFilePath = buff;
					themeFilePath = std::filesystem::path(m_EditorState->persistent.ThemeFilePath);
					themeFileName = themeFilePath.filename().string();
				}
			}
		}
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight);
		if(ImGui::Button(ICON_FA_ROTATE, ImVec2(lineHeight, lineHeight)))
		{
			if (m_EditorState->persistent.ThemeFilePath != "")
			{
				if (m_ThemeManager->LoadTheme(m_EditorState->persistent.ThemeFilePath, loadThemeStatusMessage))
				{
					loadThemeStatusMessage = "";
					themeFilePath = std::filesystem::path(m_EditorState->persistent.ThemeFilePath);
					themeFileName = themeFilePath.filename().string();
				}
			}
		}

		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Reload the theme if you made changes to the .lrtheme file");
			ImGui::EndTooltip();
		}

		if (loadThemeStatusMessage != "")
		{
			ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), loadThemeStatusMessage.c_str(), ImGui::GetContentRegionAvail().x);
			ImGui::PopTextWrapPos();
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Restore Default Theme:");
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 70);

		static bool restoreDefualtTheme = false;
		if (ImGui::Button("Restore", ImVec2(70.0f, lineHeight)))
		{
			restoreDefualtTheme = true;
		}
		ConfirmAndExecute(restoreDefualtTheme, "Restore Default Theme", "Are you sure you want to restore the default theme?", [&]() 
			{
				loadThemeStatusMessage = "";
				m_EditorState->persistent.ThemeFilePath = "";
				m_ThemeManager->LoadBuiltInDefualtTheme();
			}, m_ThemeManager, m_EditorState);

		ImGui::Dummy({ 0.0f, 20.0f });
		// Center Title
		float title_text_width = ImGui::CalcTextSize("Current Theme:").x;
		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - title_text_width) / 2.0f);
		ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f),"Current Theme:");

		// Center File Name
		std::string file_text = (themeFileName != "") ? themeFileName : "Built-in-Default";
		float file_text_width = ImGui::CalcTextSize(file_text.c_str()).x;
		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - file_text_width) / 2.0f);
		ImGui::Text(file_text.c_str());
		if (themeFileName != "" && ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
		{
			ImGui::SetTooltip(m_EditorState->persistent.ThemeFilePath.c_str());
		}

		ImGui::Dummy({ 0.0f, 20.0f });

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Create Template Theme:");
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 75.0f);
		static std::string genTemplateThemeStatusMessage = "";
		if (ImGui::Button(ICON_FA_PAINTBRUSH " Create", ImVec2(75.0f, lineHeight)))
		{
			OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
			char buff[MAX_PATH] = "template" LR_THEME_FILE_EXTENSION;

			ofn.lpstrFilter = LR_THEME_FILE_EXTENSION "Files\0 * " LR_THEME_FILE_EXTENSION "\0";
			ofn.lpstrTitle = "Save Theme File";
			ofn.nMaxFile = sizeof(buff);
			ofn.lpstrFile = buff;
			ofn.Flags = OFN_OVERWRITEPROMPT;

			if (GetSaveFileNameA(&ofn))
			{
				std::string filePath(buff);
				if (!filePath.ends_with(LR_THEME_FILE_EXTENSION))
				{ 
					filePath += std::string(LR_THEME_FILE_EXTENSION);
				}
				m_ThemeManager->SaveTheme(filePath, genTemplateThemeStatusMessage);
			}
		}
		if (genTemplateThemeStatusMessage != "")
		{
			ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), genTemplateThemeStatusMessage.c_str(), ImGui::GetContentRegionAvail().x);
			ImGui::PopTextWrapPos();
		}

		m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->DefaultButton);

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}
}