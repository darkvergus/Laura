#include "EditorTheme.h"
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

        EditorTheme newEditorTheme;
        try {
            YAML::convert<EditorTheme>::decode(node, newEditorTheme);
        }
        catch (const YAML::RepresentationException& e) {
            return { false, "YAML representation error (make sure the file is valid): " + filepath.string() + ", error: " + e.what() };
        }
        catch (const std::exception& e) {
            return { false, "Unknown error occurred while loading file: " + filepath.string() + ", error: " + e.what() };
        }
        
        *this = std::move(newEditorTheme);

        return { true, "" };
    }

    void EditorTheme::LoadDefaultDark() {
        m_ColorPallete[EditorCol_Primary1]    = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};      // HeaderHovered, ButtonHovered
        m_ColorPallete[EditorCol_Primary2]    = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};   // HeaderActive, ButtonActive, FrameBgActive
        m_ColorPallete[EditorCol_Primary3]    = ImVec4{0.01f, 0.01f, 0.01f, 1.0f};      // MenuBarBg
        m_ColorPallete[EditorCol_Secondary1]  = ImVec4{0.078f, 0.078f, 0.078f, 1.0f};   // Header, TabHovered, TabSelected, AddComponentButton
        m_ColorPallete[EditorCol_Secondary2]  = ImVec4{0.138f, 0.138f, 0.138f, 1.0f};   // SelectedHeader
        m_ColorPallete[EditorCol_Accent1]     = ImVec4{0.26f, 0.59f, 0.98f, 1.00f};     // SliderGrabActive, TabDimmedSelectedOverline (blue)
        m_ColorPallete[EditorCol_Accent2]     = ImVec4{0.391f, 0.391f, 0.391f, 1.0f};   // CheckMark, SliderGrab
        m_ColorPallete[EditorCol_Text1]       = ImVec4{1.0f, 1.0f, 1.0f, 1.0f};         // Text
        m_ColorPallete[EditorCol_Text2]       = ImVec4{0.5f, 0.5f, 0.5f, 1.0f};         // TextDisabled
        m_ColorPallete[EditorCol_Background1] = ImVec4{0.109f, 0.109f, 0.109f, 1.0f};   // WindowBg, TransformButtonBg (X/Y/Z)
        m_ColorPallete[EditorCol_Background2] = ImVec4{0.08f, 0.08f, 0.08f, 0.940f};    // PopupBg
        m_ColorPallete[EditorCol_Error]       = ImVec4{0.609f, 0.109f, 0.109f, 1.0f};   // TransformButtonXActive
        m_ColorPallete[EditorCol_Warning]     = ImVec4{1.0f, 0.5f, 0.0f, 1.0f};         // Custom fallback
        m_ColorPallete[EditorCol_Success]     = ImVec4{0.109f, 0.609f, 0.109f, 1.0f};   // TransformButtonYActive
        m_ColorPallete[EditorCol_X]           = ImVec4{0.409f, 0.109f, 0.109f, 1.0f};   // TransformButtonXHovered
        m_ColorPallete[EditorCol_Y]           = ImVec4{0.109f, 0.409f, 0.109f, 1.0f};   // TransformButtonYHovered
        m_ColorPallete[EditorCol_Z]           = ImVec4{0.109f, 0.109f, 0.409f, 1.0f};   // TransformButtonZHovered
        ApplyAllToImgui();
    }

    void EditorTheme::LoadDefaultLight() {
        m_ColorPallete[EditorCol_Primary1]    = ImVec4{0.85f, 0.85f, 0.86f, 1.0f};       // HeaderHovered, ButtonHovered
        m_ColorPallete[EditorCol_Primary2]    = ImVec4{0.75f, 0.75f, 0.76f, 1.0f};       // HeaderActive, ButtonActive, FrameBgActive
        m_ColorPallete[EditorCol_Primary3]    = ImVec4{0.9f, 0.9f, 0.9f, 1.0f};          // MenuBarBg
        m_ColorPallete[EditorCol_Secondary1]  = ImVec4{0.93f, 0.93f, 0.93f, 1.0f};       // Header, TabHovered, TabSelected, AddComponentButton
        m_ColorPallete[EditorCol_Secondary2]  = ImVec4{0.82f, 0.82f, 0.82f, 1.0f};       // SelectedHeader
        m_ColorPallete[EditorCol_Accent1]     = ImVec4{0.2f, 0.6f, 0.15f, 1.0f};         // SliderGrabActive (greenish)
        m_ColorPallete[EditorCol_Accent2]     = ImVec4{0.3f, 0.3f, 0.3f, 1.0f};          // CheckMark, SliderGrab
        m_ColorPallete[EditorCol_Text1]       = ImVec4{0.0f, 0.0f, 0.0f, 1.0f};          // Text (black)
        m_ColorPallete[EditorCol_Text2]       = ImVec4{0.4f, 0.4f, 0.4f, 1.0f};          // TextDisabled
        m_ColorPallete[EditorCol_Background1] = ImVec4{0.98f, 0.98f, 0.98f, 1.0f};       // WindowBg
        m_ColorPallete[EditorCol_Background2] = ImVec4{0.95f, 0.95f, 0.95f, 1.0f};       // PopupBg
        m_ColorPallete[EditorCol_Error]       = ImVec4{0.8f, 0.2f, 0.2f, 1.0f};          // TransformButtonXActive
        m_ColorPallete[EditorCol_Warning]     = ImVec4{1.0f, 0.65f, 0.0f, 1.0f};         // Orange
        m_ColorPallete[EditorCol_Success]     = ImVec4{0.2f, 0.6f, 0.2f, 1.0f};          // Green
        m_ColorPallete[EditorCol_X]           = ImVec4{0.8f, 0.2f, 0.2f, 1.0f};          // TransformButtonXHovered
        m_ColorPallete[EditorCol_Y]           = ImVec4{0.2f, 0.8f, 0.2f, 1.0f};          // TransformButtonYHovered
        m_ColorPallete[EditorCol_Z]           = ImVec4{0.2f, 0.2f, 0.8f, 1.0f};          // TransformButtonZHovered
        ApplyAllToImgui();
    }
    void EditorTheme::ApplyAllToImgui() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_WindowBg]                  = m_ColorPallete[EditorCol_Background1];
    style.Colors[ImGuiCol_PopupBg]                   = m_ColorPallete[EditorCol_Background2];
    style.Colors[ImGuiCol_Border]                    = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_Header]                    = m_ColorPallete[EditorCol_Primary3];
    style.Colors[ImGuiCol_HeaderHovered]             = m_ColorPallete[EditorCol_Primary2];
    style.Colors[ImGuiCol_HeaderActive]              = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_Button]                    = m_ColorPallete[EditorCol_Secondary1];
    style.Colors[ImGuiCol_ButtonHovered]             = m_ColorPallete[EditorCol_Primary1];
    style.Colors[ImGuiCol_ButtonActive]              = m_ColorPallete[EditorCol_Primary2];
    style.Colors[ImGuiCol_CheckMark]                 = m_ColorPallete[EditorCol_Accent2];
    style.Colors[ImGuiCol_SliderGrab]                = m_ColorPallete[EditorCol_Accent2];
    style.Colors[ImGuiCol_SliderGrabActive]          = m_ColorPallete[EditorCol_Accent1];
    style.Colors[ImGuiCol_FrameBg]                   = m_ColorPallete[EditorCol_Background1];
    style.Colors[ImGuiCol_FrameBgHovered]            = m_ColorPallete[EditorCol_Background1];
    style.Colors[ImGuiCol_FrameBgActive]             = m_ColorPallete[EditorCol_Primary2];
    style.Colors[ImGuiCol_Tab]                       = m_ColorPallete[EditorCol_Background2];
    style.Colors[ImGuiCol_TabHovered]                = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_TabActive]                 = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_TabSelectedOverline]       = m_ColorPallete[EditorCol_Accent1];
    style.Colors[ImGuiCol_TabDimmedSelectedOverline] = m_ColorPallete[EditorCol_Primary1];
    style.Colors[ImGuiCol_TabUnfocused]              = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_TabUnfocusedActive]        = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_TitleBg]                   = m_ColorPallete[EditorCol_Background2];
    style.Colors[ImGuiCol_TitleBgActive]             = m_ColorPallete[EditorCol_Background2];
    style.Colors[ImGuiCol_TitleBgCollapsed]          = m_ColorPallete[EditorCol_Background2];
    style.Colors[ImGuiCol_ScrollbarGrab]             = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_ResizeGrip]                = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_ResizeGripHovered]         = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_ResizeGripActive]          = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_Separator]                 = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_SeparatorHovered]          = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_SeparatorActive]           = m_ColorPallete[EditorCol_Secondary2];
    style.Colors[ImGuiCol_Text]                      = m_ColorPallete[EditorCol_Text1];
    style.Colors[ImGuiCol_TextDisabled]              = m_ColorPallete[EditorCol_Text2];
    style.Colors[ImGuiCol_MenuBarBg]                 = m_ColorPallete[EditorCol_Primary3];
}

}