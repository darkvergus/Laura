#include "EditorTheme.h"
#include <fstream>

namespace Laura
{
    ThemeManager::ThemeManager()
    {
        m_ActiveTheme = std::make_shared<Theme>();
    }




	bool ThemeManager::SerializeTheme(std::shared_ptr<Theme> theme, const std::string& filepath, std::string& statusMessage)
	{
        // Check for the correct file extension
        if (!filepath.ends_with(LR_THEME_FILE_EXTENSION))
        {
            statusMessage = "Invalid file extension for theme file ( or none selected ): " + filepath;
            return false;
        }

        // Create a file and Check for permission issues, non-existent directory, or another I/O error
		std::ofstream fout(filepath);
		if (!fout.is_open()) 
        {
            statusMessage = "Could not open file for writing: " + filepath;
            return false;
        }

        YAML::Node node;
        // Serialize the theme to YAML
        try
        {
             node = YAML::convert<Theme>::encode(*theme);
        }
        catch (const YAML::RepresentationException& e)
        {
            statusMessage = "YAML representation error (make sure the file is valid): " + filepath + ", error: " + e.what();
			return false;
		}
		catch (const std::exception& e)
        {
            statusMessage = "Unknown error occurred while saving file: " + filepath + ", error: " + e.what();
			return false;
		}

        fout << node;
        return true;
	}




	bool ThemeManager::DeserializeTheme(std::shared_ptr<Theme> themeOut, const std::string& filepath, std::string& statusMessage)
	{
        // Check for the correct file extension
        if (!filepath.ends_with(LR_THEME_FILE_EXTENSION))
        {
            statusMessage = "Invalid file extension for theme file (or none selected): " + filepath;
			return false;
		}
        
        // Check if the file exists and can be opened
        std::ifstream file(filepath);
        if (!file.is_open()) 
        {
            statusMessage = "Could not open file: " + filepath;
            return false;
        }
        file.close();
	
        YAML::Node node;
        try 
        {
            node = YAML::LoadFile(filepath);
        }
        catch (const YAML::BadFile& e) 
        {
            statusMessage = "Failed to load file: " + filepath;
            return false;
        }
        catch (const YAML::ParserException& e) 
        {
            statusMessage = "Failed to parse file: " + filepath;
            return false;
        }
        catch (const std::exception& e) 
        {
            statusMessage = "Unknown error occurred while loading file: " + filepath + ", error: " + e.what();
            return false;
        }

		Theme theme; // Temporary theme object
        try
        {
            YAML::convert<Theme>::decode(node, theme);
        }
        catch (const YAML::RepresentationException& e) 
        {
			statusMessage = "YAML representation error (make sure the file is valid): " + filepath + ", error: " + e.what();
			return false;
		}
		catch (const std::exception& e) 
        {
            statusMessage = "Unknown error occurred while loading file: " + filepath + ", error: " + e.what();
			return false;
		}
        *themeOut = theme; // Copy the temporary theme to the active theme after catching exceptions
		
        return true;
	}





    void ThemeManager::ImGuiSetAll()
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