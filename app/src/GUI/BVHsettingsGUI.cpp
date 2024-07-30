#include "GUI/BVHsettingsGUI.h"

void BVH_settings_GUI(bool& display_BVH, int BVH_tree_depth, int& heatmap_color_limit, bool& showPixelData, bool& was_IMGUI_input, bool disabled) {
    ImGuiWindowFlags BVH_window_flags = 0;
    BVH_window_flags |= ImGuiWindowFlags_NoCollapse;
    BVH_window_flags |= ImGuiWindowFlags_NoTitleBar;

    if (disabled) { ImGui::BeginDisabled(); }
    ImGui::Begin("BVH Settings", NULL, BVH_window_flags);

    ImGui::Text("BVH Tree Depth: %d", BVH_tree_depth);

    ImGui::SeparatorText("Visual");
    if (ImGui::Checkbox("Show BVH heatmap", &display_BVH)) {
        was_IMGUI_input = true;
        showPixelData = true;
    }

    IMGUI_INPUT(ImGui::SliderInt("BVH Heatmap intersections limit", &heatmap_color_limit, 10.0f, 10000.0f, NULL, ImGuiSliderFlags_Logarithmic));

    if (!display_BVH) {
        ImGui::BeginDisabled();
        showPixelData = false;
    }
    ImGui::Checkbox("Show pixel-data", &showPixelData);
    if (!display_BVH) { ImGui::EndDisabled(); }

    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted("The 'Show BVH' option must be ENABLED to display pixel-data. When this option is enabled and you hover over pixels in the viewport, a tooltip will appear. The tooltip provides detailed information about the pixel under the cursor, including its position, color values and the number of bounces.");

        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    ImGui::End();
    if (disabled) { ImGui::EndDisabled(); }
}