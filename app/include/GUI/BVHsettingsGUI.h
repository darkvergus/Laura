#pragma once

#include <imgui.h>

/**
* @brief Wrap code in an if statement and set imgui_was_input as true
* @param code - the code to be wrapped
*/
#define IMGUI_INPUT(code) \
    if (code) { \
        was_IMGUI_input = true; \
    }


/**
* @brief GUI for BVH settings
* @param display_BVH - whether to display the BVH
* @param active_heuristic - the heuristic to use for BVH construction
* @param displayed_layer - the layer of the BVH to display
* @param display_multiple - whether to display multiple layers of the BVH (till the displayed layer)
* @param was_IMGUI_input - whether there was IMGUI input (used in shader to tell when to restart the accumulation of rays)
* @param disabled - to disable the GUI when in the camera control mode
* */
void BVH_settings_GUI(bool& display_BVH, int BVH_tree_depth, int& heatmap_color_limit, bool& showPixelData, bool& was_IMGUI_input, bool disabled);
