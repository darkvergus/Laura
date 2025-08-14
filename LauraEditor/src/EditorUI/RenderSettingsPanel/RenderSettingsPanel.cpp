#include <IconsFontAwesome6.h>
#include "EditorUI/RenderSettingsPanel/RenderSettingsPanel.h"
#include "EditorUI/UtilityUI.h"

namespace Laura 
{

	void RenderSettingsPanel::init() {
		// sync the renderer with the newely deserialized render settings 
		m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(m_EditorState->persistent.editorRenderSettings));
	}

	void RenderSettingsPanel::OnImGuiRender() {
		EditorTheme& theme = m_EditorState->temp.editorTheme;


		auto DrawLabel = [&theme](const char* label) {
            theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
            ImGui::Text("%s", label);
            theme.PopColor();
            ImGui::SameLine(150);
        };

		ImGui::Begin(ICON_FA_WRENCH " RENDER SETTINGS");
		if (m_EditorState->temp.isInRuntimeMode) {
			ImGui::BeginDisabled();
		}
		float avail_width = ImGui::GetContentRegionAvail().x;
		float margin_right = 5.0f;  // pixels to leave empty on the right

		ImGui::BeginChild("child_with_margin", ImVec2(avail_width - margin_right, 0), false);
		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)) {
			if (ImGui::BeginTabItem("Editor")) {
				RenderSettings& editorRenderSettings = m_EditorState->persistent.editorRenderSettings;
				theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
				ImGui::Text("General");
				theme.PopColor();

				DrawLabel("Resolution:");
				static int current_idx = 0;
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo("##resolution", m_ResolutionOptions[current_idx].label)) {
    				for (int n = 0; n < m_ResolutionOptions.size(); n++) {
        				bool selected = (current_idx == n);
        				if (ImGui::Selectable(m_ResolutionOptions[n].label, selected)) {
            				current_idx = n;

							editorRenderSettings.resolution = m_ResolutionOptions[n].resolution;
							m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editorRenderSettings));
        				}
						if (selected) { ImGui::SetItemDefaultFocus(); }
    				}
    				ImGui::EndCombo();
				}

				float drag_speed = 0.1f;  // slower dragging speed
				DrawLabel("Rays Per Pixel:");
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::DragInt("##RaysPerPixelDragInt", &editorRenderSettings.raysPerPixel, drag_speed, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp)) {
					m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editorRenderSettings));
				}

				DrawLabel("Bounces Per Ray:");
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::DragInt("##BouncesPerRayDragInt", &editorRenderSettings.bouncesPerRay, drag_speed, 0, 100, "%d", ImGuiSliderFlags_AlwaysClamp)) {
					m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editorRenderSettings));
				}

				theme.PushColor(ImGuiCol_CheckMark, EditorCol_Text1);
				{
					DrawLabel("Light Accumulation:");
					if (ImGui::Checkbox("##LightAccumulationCheckbox", &editorRenderSettings.accumulate)) {
						m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editorRenderSettings));
					}

					DrawLabel("VSync:");
					if (ImGui::Checkbox("##VSyncCheckbox", &editorRenderSettings.vSync)) {
						m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editorRenderSettings));
					}

					ImGui::Dummy({ 0.0f, 10.0f });
					theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
					ImGui::Text("Debug");
					theme.PopColor();

					DrawLabel("Visualize BVH:");
					if (ImGui::Checkbox("##VisualizeBVHCheckbox", &editorRenderSettings.showBvhHeatmap)) {
						m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editorRenderSettings));
					}
				}
				theme.PopColor();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Runtime")) {
				RenderSettings& runtimeRenderSettings = m_ProjectManager->GetMutableRuntimeRenderSettings();
				theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
				ImGui::Text("General");
				theme.PopColor();
				
				DrawLabel("Resolution:");
				static int current_idx = 0;
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo("##resolution", m_ResolutionOptions[current_idx].label)) {
    				for (int n = 0; n < m_ResolutionOptions.size(); n++) {
        				bool selected = (current_idx == n);
        				if (ImGui::Selectable(m_ResolutionOptions[n].label, selected)) {
            				current_idx = n;

							runtimeRenderSettings.resolution = m_ResolutionOptions[n].resolution;
        				}
						if (selected) { ImGui::SetItemDefaultFocus(); }
    				}
    				ImGui::EndCombo();
				}

				float drag_speed = 0.1f;  // slower dragging speed
				DrawLabel("Rays Per Pixel:");
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::DragInt("##RaysPerPixelDragInt", &runtimeRenderSettings.raysPerPixel, drag_speed, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);

				DrawLabel("Bounces Per Ray:");
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::DragInt("##BouncesPerRayDragInt", &runtimeRenderSettings.bouncesPerRay, drag_speed, 0, 100, "%d", ImGuiSliderFlags_AlwaysClamp);

				theme.PushColor(ImGuiCol_CheckMark, EditorCol_Text1);
				{
					DrawLabel("Light Accumulation:");
					ImGui::Checkbox("##LightAccumulationCheckbox", &runtimeRenderSettings.accumulate);

					DrawLabel("VSync:");
					ImGui::Checkbox("##VSyncCheckbox", &runtimeRenderSettings.vSync);
				}
				theme.PopColor();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndChild();
		
		if (m_EditorState->temp.isInRuntimeMode) {
			ImGui::EndDisabled();
		}
		
		ImGui::End();
	}
 }