#include <IconsFontAwesome6.h>
#include "EditorUI\RenderSettingsPanel\RenderSettingsPanel.h"

namespace Laura 
{

	void RenderSettingsPanel::OnImGuiRender() {
		EditorTheme& theme = m_EditorState->temp.editorTheme;

		auto DrawLabel = [&theme](const char* label) {
            theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
            ImGui::Text("%s", label);
            theme.PopColor();
            ImGui::SameLine(150);
        };

		ImGui::Begin(ICON_FA_WRENCH " Render Settings");
		float avail_width = ImGui::GetContentRegionAvail().x;
		float margin_right = 5.0f;  // pixels to leave empty on the right

		ImGui::BeginChild("child_with_margin", ImVec2(avail_width - margin_right, 0), false);
		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)) {

			if (ImGui::BeginTabItem("Editor")) {
				theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
				ImGui::Text("General");
				theme.PopColor();

				DrawLabel("Resolution:");
				static int current_idx = 0;
				if (ImGui::BeginCombo("##resolution", m_ResolutionOptions[current_idx].label, ImGuiComboFlags_NoArrowButton)) {
    				for (int n = 0; n < m_ResolutionOptions.size(); n++) {
        				bool selected = (current_idx == n);
        				if (ImGui::Selectable(m_ResolutionOptions[n].label, selected)) {
            				current_idx = n;
        				}
						if (selected) { ImGui::SetItemDefaultFocus(); }
    				}
    				ImGui::EndCombo();
				}

				float drag_speed = 0.1f;  // slower dragging speed
				DrawLabel("Rays Per Pixel:");
				ImGui::DragInt("##RaysPerPixelDragInt", &m_RaysPerPixel, drag_speed, 0, 100, "%d", ImGuiSliderFlags_AlwaysClamp);

				DrawLabel("Bounces Per Ray:");
				ImGui::DragInt("##BouncesPerRayDragInt", &m_BouncesPerRay, drag_speed, 0, 100, "%d", ImGuiSliderFlags_AlwaysClamp);

				theme.PushColor(ImGuiCol_CheckMark, EditorCol_Text1);
				{
					DrawLabel("Light Accumulation:");
					ImGui::Checkbox("##LightAccumulationCheckbox", &m_IsAccumulate);

					DrawLabel("VSync:");
					ImGui::Checkbox("##VSyncCheckbox", &m_IsVSync);

					ImGui::Dummy({ 0.0f, 10.0f });
					theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
					ImGui::Text("Debug");
					theme.PopColor();

					DrawLabel("Visualize BVH:");
					ImGui::Checkbox("##VisualizeBVHCheckbox", &m_IsBVHVisualization);
				}
				theme.PopColor();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Runtime")) {
				theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
				ImGui::Text("General");
				theme.PopColor();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndChild();
		ImGui::End();
	}
 }