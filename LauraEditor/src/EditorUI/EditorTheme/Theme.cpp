#include "Theme.h"
#include <fstream>

namespace Laura
{
    std::pair<bool, std::string> EditorTheme::SerializeToYAML(const std::filesystem::path& filepath) {
        if (filepath.extension() != EDITOR_THEME_FILE_EXTENSION) {
            return { false, "Invalid file extension for theme file (or none selected): " + filepath.string() };
        }

        std::ofstream fout(filepath);
        if (!fout.is_open()) {
            return { false, "Could not open file for writing: " + filepath.string() };
        }

        YAML::Node node;
        try {
            node = YAML::convert<EditorTheme>::encode(*this);
        }
        catch (const YAML::RepresentationException& e) {
            return { false, "YAML representation error (make sure the file is valid): " + filepath.string() + ", error: " + e.what() };
        }
        catch (const std::exception& e) {
            return { false, "Unknown error occurred while saving file: " + filepath.string() + ", error: " + e.what() };
        }

        fout << node;
        fout.close();
        return { true, "" };
    }

    std::pair<bool, std::string> EditorTheme::DeserializeFromYAML(const std::filesystem::path& filepath) {
        if (filepath.extension() != EDITOR_THEME_FILE_EXTENSION) {
            return { false, "Invalid file extension for theme file (or none selected): " + filepath.string() };
        }

        YAML::Node node;
        try {
            node = YAML::LoadFile(filepath.string());
        }
        catch (const YAML::BadFile& e) {
            return { false, "Failed to load file: " + filepath.string() };
        }
        catch (const YAML::ParserException& e) {
            return { false, "Failed to parse file: " + filepath.string() };
        }
        catch (const std::exception& e) {
            return { false, "Unknown error occurred while loading file: " + filepath.string() + ", error: " + e.what() };
        }

        EditorTheme tmpTheme;
        try {
            YAML::convert<EditorTheme>::decode(node, tmpTheme);
        }
        catch (const YAML::RepresentationException& e) {
            return { false, "YAML representation error (make sure the file is valid): " + filepath.string() + ", error: " + e.what() };
        }
        catch (const std::exception& e) {
            return { false, "Unknown error occurred while loading file: " + filepath.string() + ", error: " + e.what() };
        }

        size_t numPushedColsBeforeLoad = m_ColorStack.size();
        *this = tmpTheme;
        for (size_t i = 0; i < numPushedColsBeforeLoad; i++) {
            PushColor(ImGuiCol_Text, TEXT_1); // populate with dummy values ( to prevent push/pop imbalance )
        }

        return { true, "" };
    }

    void EditorTheme::ApplyThemeToImgui() {
        ImGuiStyle& style = ImGui::GetStyle();

        // Backgrounds
        style.Colors[ImGuiCol_WindowBg]             = BACKGROUND_1;
        style.Colors[ImGuiCol_PopupBg]              = BACKGROUND_2;
        // Borders
        style.Colors[ImGuiCol_Border]               = SECONDARY_2;
        // Headers
        style.Colors[ImGuiCol_Header]               = SECONDARY_1;
        style.Colors[ImGuiCol_HeaderHovered]        = PRIMARY_1;
        style.Colors[ImGuiCol_HeaderActive]         = PRIMARY_2;
        // Buttons
        style.Colors[ImGuiCol_Button]               = SECONDARY_1;
        style.Colors[ImGuiCol_ButtonHovered]        = PRIMARY_1;
        style.Colors[ImGuiCol_ButtonActive]         = PRIMARY_2;
        // Checkmarks
        style.Colors[ImGuiCol_CheckMark]            = ACCENT_2;
        // Sliders
        style.Colors[ImGuiCol_SliderGrab]           = ACCENT_2;
        style.Colors[ImGuiCol_SliderGrabActive]     = ACCENT_1;
        // Frame Background
        style.Colors[ImGuiCol_FrameBg]              = BACKGROUND_1;
        style.Colors[ImGuiCol_FrameBgHovered]       = BACKGROUND_1;
        style.Colors[ImGuiCol_FrameBgActive]        = PRIMARY_2;
        // Tabs
        style.Colors[ImGuiCol_Tab]                  = BACKGROUND_1;
        style.Colors[ImGuiCol_TabHovered]           = SECONDARY_1;
        style.Colors[ImGuiCol_TabActive]            = SECONDARY_1;
        style.Colors[ImGuiCol_TabUnfocused]         = SECONDARY_1;
        style.Colors[ImGuiCol_TabUnfocusedActive]   = SECONDARY_2;
        // Title
        style.Colors[ImGuiCol_TitleBg]              = BACKGROUND_2;
        style.Colors[ImGuiCol_TitleBgActive]        = BACKGROUND_2;
        style.Colors[ImGuiCol_TitleBgCollapsed]     = BACKGROUND_2;
        // Scrollbar
        style.Colors[ImGuiCol_ScrollbarGrab]        = SECONDARY_2;
        // Grips
        style.Colors[ImGuiCol_ResizeGrip]           = SECONDARY_2;
        style.Colors[ImGuiCol_ResizeGripHovered]    = SECONDARY_2;
        style.Colors[ImGuiCol_ResizeGripActive]     = SECONDARY_2;
        // Separator
        style.Colors[ImGuiCol_Separator]            = SECONDARY_2;
        style.Colors[ImGuiCol_SeparatorHovered]     = SECONDARY_2;
        style.Colors[ImGuiCol_SeparatorActive]      = SECONDARY_2;
        // Text
        style.Colors[ImGuiCol_Text]                 = TEXT_1;
        style.Colors[ImGuiCol_TextDisabled]         = TEXT_2;
        // Transform Widget
        style.Colors[ImGuiCol_Button]               = SECONDARY_1;
        style.Colors[ImGuiCol_ButtonHovered]        = SECONDARY_1;
        style.Colors[ImGuiCol_ButtonActive]         = SECONDARY_2;
    }
}