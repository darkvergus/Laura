#include "ThemesPanel.h"
#include <IconsFontAwesome6.h>

namespace Laura
{
	void ThemesPanel::OnImGuiRender(std::shared_ptr<EditorState> editorState, std::shared_ptr<ThemeManager> themeManager)
	{
		ImGuiWindowFlags ThemesPanelFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
		ImGui::Begin(ICON_FA_BRUSH " Themes", &editorState->temp.ThemeSettingsPanelOpen, ThemesPanelFlags);

		ImGui::Text("Active Theme: %s", editorState->persistent.ThemeFilePath.c_str());
		
		if (ImGui::Button("Open")) {

			std::string cachedPath = editorState->persistent.ThemeFilePath;

			OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
			char buff[MAX_PATH] = {};
			ofn.lpstrFilter = LR_THEME_FILE_EXTENSION "Files\0 * " LR_THEME_FILE_EXTENSION "\0";
			ofn.lpstrTitle = "Select Theme:";
			ofn.nMaxFile = sizeof(buff);
			ofn.lpstrFile = buff;
			if (GetOpenFileNameA(&ofn)) 
			{
				editorState->persistent.ThemeFilePath = buff;

				if (editorState->persistent.ThemeFilePath != cachedPath)
				{
					std::string statusMessage;
					if (!themeManager->LoadTheme(editorState->persistent.ThemeFilePath, statusMessage))
					{
						editorState->persistent.ThemeFilePath = "";
					}
				}
			}
		}


		if (ImGui::Button("Reload")) 
		{
			std::string statusMessage;
			themeManager->LoadTheme(editorState->persistent.ThemeFilePath, statusMessage);
		}


		if (ImGui::Button("Generate Default"))
		{
			OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
			char buff[MAX_PATH] = "default" LR_THEME_FILE_EXTENSION;

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
				std::string statusMessage;
				themeManager->GenerateDefaultThemeFile(filePath, statusMessage);
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}
}