#include "EditorTheme.h"
#include <fstream>

namespace Laura
{
    ThemeManager::ThemeManager()
    {
        m_ActiveTheme = std::make_shared<Theme>();
    }

	bool ThemeManager::SerializeTheme(std::shared_ptr<Theme> theme, const std::string& filepath)
	{
        if (!filepath.ends_with(".lauratheme"))
        {
            LR_APP_WARN("Invalid file extension for theme file: {0}", filepath);
            return false;
        }

		std::ofstream fout(filepath);
		if (!fout.is_open()) {
			LR_APP_CRITICAL("Could not open file for writing: {0}", filepath);
            return false;
        }

		YAML::Node node = YAML::convert<Theme>::encode(*theme);
		
        fout << node;
        return true;
	}

	bool ThemeManager::DeserializeTheme(std::shared_ptr<Theme> themeOut, const std::string& filepath)
	{
        if (!filepath.ends_with(".lauratheme"))
        {
			LR_APP_WARN("Invalid file extension for theme file: {0}", filepath);
			return false;
		}
        
        std::ifstream file(filepath);
        if (!file.is_open()) {
            LR_APP_WARN("Could not open file: {0}", filepath);
            return false;
        }
        file.close();
	
        YAML::Node node;
        try {
            node = YAML::LoadFile(filepath);
        }
        catch (const YAML::BadFile& e) {
            LR_APP_WARN("Failed to load file: {0}, error: {1}", filepath, e.what());
            return false;
        }
        catch (const YAML::ParserException& e) {
            LR_APP_WARN("YAML parsing error in file: {0}, error: {1}", filepath, e.what());
            return false;
        }
        catch (const std::exception& e) {
            LR_APP_WARN("Unknown error occurred while loading file: {0}, error: {1}", filepath, e.what());
            return false;
        }

		Theme theme;

		if (!YAML::convert<Theme>::decode(node, theme)) {
			LR_APP_WARN("Could not deserialize theme from file (invalid file content): {0}", filepath);
            return false;
		}

        *themeOut = theme;
		return true;
	}

    void ThemeManager::SetThemeDefaults()
    {
        ImVec4* colors = ImGui::GetStyle().Colors;

        // Backgrounds
        colors[ImGuiCol_WindowBg] = m_ActiveTheme->DefaultWindowBg;
        colors[ImGuiCol_PopupBg] = m_ActiveTheme->DefaultPopupBg;
        // Borders
        colors[ImGuiCol_Border] = m_ActiveTheme->DefaultBorder;
        // Headers
        colors[ImGuiCol_Header] = m_ActiveTheme->DefaultHeader;
        colors[ImGuiCol_HeaderHovered] = m_ActiveTheme->DefaultHeaderHovered;
        colors[ImGuiCol_HeaderActive] = m_ActiveTheme->DefaultHeaderActive;
        // Buttons
        colors[ImGuiCol_Button] = m_ActiveTheme->DefaultButton;
        colors[ImGuiCol_ButtonHovered] = m_ActiveTheme->DefaultButtonHovered;
        colors[ImGuiCol_ButtonActive] = m_ActiveTheme->DefaultButtonActive;
        // Checkmarks
        colors[ImGuiCol_CheckMark] = m_ActiveTheme->DefaultCheckMark;
        // Sliders
        colors[ImGuiCol_SliderGrab] = m_ActiveTheme->DefaultSliderGrab;
        colors[ImGuiCol_SliderGrabActive] = m_ActiveTheme->DefaultSliderGrabActive;
        // Frame BG
        colors[ImGuiCol_FrameBg] = m_ActiveTheme->DefaultFrameBg;
        colors[ImGuiCol_FrameBgHovered] = m_ActiveTheme->DefaultFrameBgHovered;
        colors[ImGuiCol_FrameBgActive] = m_ActiveTheme->DefaultFrameBgActive;
        // Tabs
        colors[ImGuiCol_Tab] = m_ActiveTheme->DefaultTab;
        colors[ImGuiCol_TabHovered] = m_ActiveTheme->DefaultTabHovered;
        colors[ImGuiCol_TabActive] = m_ActiveTheme->DefaultTabSelected;
        colors[ImGuiCol_TabSelectedOverline] = m_ActiveTheme->DefaultTabSelectedOverline;
        colors[ImGuiCol_TabDimmed] = m_ActiveTheme->DefaultTabDimmed;
        colors[ImGuiCol_TabDimmedSelected] = m_ActiveTheme->DefaultTabDimmedSelected;
        colors[ImGuiCol_TabDimmedSelectedOverline] = m_ActiveTheme->DefaultTabDimmedSelectedOverline;
        // Title
        colors[ImGuiCol_TitleBg] = m_ActiveTheme->DefaultTitleBg;
        colors[ImGuiCol_TitleBgActive] = m_ActiveTheme->DefaultTitleBgActive;
        colors[ImGuiCol_TitleBgCollapsed] = m_ActiveTheme->DefaultTitleBgCollapsed;
        // Scrollbar
        colors[ImGuiCol_ScrollbarGrab] = m_ActiveTheme->DefaultScrollbarGrab;
        // Resize Grips
        colors[ImGuiCol_ResizeGrip] = m_ActiveTheme->DefaultResizeGrip;
        colors[ImGuiCol_ResizeGripHovered] = m_ActiveTheme->DefaultResizeGripHovered;
        colors[ImGuiCol_ResizeGripActive] = m_ActiveTheme->DefaultResizeGripActive;
        // Separators
        colors[ImGuiCol_Separator] = m_ActiveTheme->DefaultSeparator;
        colors[ImGuiCol_SeparatorHovered] = m_ActiveTheme->DefaultSeparatorHovered;
        colors[ImGuiCol_SeparatorActive] = m_ActiveTheme->DefaultSeparatorActive;
        // Text
        colors[ImGuiCol_Text] = m_ActiveTheme->DefaultText;
        colors[ImGuiCol_TextDisabled] = m_ActiveTheme->DefaultTextDisabled;
    }

}