#include "ThemesPanel.h"
#include <IconsFontAwesome6.h>

namespace Laura
{
	void ThemesPanel::OnImGuiRender(std::shared_ptr<EditorState> editorState, std::shared_ptr<ThemeManager> themeManager)
	{
		ImGuiWindowFlags ThemesPanelFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
		ImGui::Begin(ICON_FA_BRUSH " Themes", &editorState->ThemeSettingsPanelOpen, ThemesPanelFlags);

		ImGui::Text("Active Theme: %s", editorState->ActiveThemeFilePath.c_str());
		
		if (ImGui::Button("Open")) {

			std::string cachedPath = editorState->ActiveThemeFilePath;

			OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
			char buff[MAX_PATH] = {};
			ofn.lpstrFilter = "lauratheme Files\0*.lauratheme*\0";
			ofn.lpstrTitle = "Select a theme";
			ofn.nMaxFile = sizeof(buff);
			ofn.lpstrFile = buff;
			if (GetOpenFileNameA(&ofn)) 
			{
				editorState->ActiveThemeFilePath = buff;

				if (editorState->ActiveThemeFilePath != cachedPath)
				{
					themeManager->LoadTheme(editorState->ActiveThemeFilePath);
				}
			}
		}


		if (ImGui::Button("Reload")) 
		{
			themeManager->LoadTheme(editorState->ActiveThemeFilePath);
		}


		if (ImGui::Button("Generate Default"))
		{
			OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
			char buff[MAX_PATH] = "Default.lauratheme";

			ofn.lpstrFilter = "lauratheme Files\0 * .lauratheme\0";
			ofn.lpstrTitle = "Save Theme File";
			ofn.nMaxFile = sizeof(buff);
			ofn.lpstrFile = buff;
			ofn.Flags = OFN_OVERWRITEPROMPT;

			if (GetSaveFileNameA(&ofn))
			{
				std::string filePath(buff);
				if (!filePath.ends_with(".lauratheme")) 
				{ 
					filePath += std::string(".lauratheme"); 
				}
				themeManager->GenerateDefaultThemeFile(filePath);
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}
}