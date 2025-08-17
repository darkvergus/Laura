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

		auto DrawSection = [&theme](const char* label) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			theme.PushColor(ImGuiCol_Text, EditorCol_Accent1);
			ImGui::Text("%s", label);
			theme.PopColor();
			ImGui::TableSetColumnIndex(1);
			ImGui::TableSetColumnIndex(2);
		};

		auto DrawLabel = [&theme](const char* label) {
			theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
			ImGui::Text("%s", label);
			theme.PopColor();
		};

		auto CellLabelCentered = [](const char* label) {
			float columnWidth = ImGui::GetColumnWidth();
			float textWidth = ImGui::CalcTextSize(label).x;
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (columnWidth - textWidth) * 0.5f);
			ImGui::Text("%s", label);
		};

		ImGui::Begin(ICON_FA_WRENCH " Render Settings");

		if (m_EditorState->temp.isInRuntimeSimulation) {
			ImGui::BeginDisabled();
		}

		float avail_width = ImGui::GetContentRegionAvail().x;
		ImGui::BeginChild("child_with_margin", ImVec2(avail_width - 5.0f, 0), false);

		RenderSettings& editor = m_EditorState->persistent.editorRenderSettings;
		RenderSettings& runtime = m_ProjectManager->GetMutableRuntimeRenderSettings();

	
		if (ImGui::BeginTable("RenderSettingsTable", 3)) {
			ImGui::TableSetupColumn("##Setting", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Editor", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Runtime", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted("");
			ImGui::TableSetColumnIndex(1);
			CellLabelCentered("Editor");
			ImGui::TableSetColumnIndex(2);
			CellLabelCentered("Runtime");

			// -----------------------------
			// Quality section
			// -----------------------------
			DrawSection("Quality");


			// Resolution row
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0); 
			DrawLabel("Resolution");
			ImGui::TableSetColumnIndex(1);
			{
				ImGui::PushID("##EditorResolution");
				int current_idx = 0;
				for (int i = 0; i < m_ResolutionOptions.size(); i++) {
					if (m_ResolutionOptions[i].resolution == editor.resolution) { current_idx = i; break; }
				}
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo("##Combo", m_ResolutionOptions[current_idx].label, ImGuiComboFlags_HeightLarge)) {
					for (int n = 0; n < m_ResolutionOptions.size(); n++) {
						const auto& label = m_ResolutionOptions[n].label;
						if (m_ResolutionOptions[n].resolution == glm::uvec2(0, 0)) {
							if (n != 0) { ImGui::TextDisabled(""); } // empty line
							ImGui::TextDisabled("%s", label);
							continue;
						}
						bool selected = (current_idx == n);
						if (ImGui::Selectable(label, selected)) {
							current_idx = n;
							editor.resolution = m_ResolutionOptions[n].resolution;
							m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editor));
						}
						if (selected) { ImGui::SetItemDefaultFocus(); }
					}
					ImGui::EndCombo();
				}
				ImGui::PopID();
			}
			ImGui::TableSetColumnIndex(2);
			{
				ImGui::PushID("##RuntimeResolution");
				int current_idx = 0;
				for (int i = 0; i < m_ResolutionOptions.size(); i++) {
					if (m_ResolutionOptions[i].resolution == runtime.resolution) { current_idx = i; break; }
				}
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo("##Combo", m_ResolutionOptions[current_idx].label, ImGuiComboFlags_HeightLarge)) {
					for (int n = 0; n < m_ResolutionOptions.size(); n++) {
						const auto& label = m_ResolutionOptions[n].label;
						if (m_ResolutionOptions[n].resolution == glm::uvec2(0, 0)) {
							if (n != 0) { ImGui::TextDisabled(""); } // empty line
							ImGui::TextDisabled("%s", label);
							continue;
						}
						bool selected = (current_idx == n);
						if (ImGui::Selectable(m_ResolutionOptions[n].label, selected)) {
							current_idx = n;
							runtime.resolution = m_ResolutionOptions[n].resolution;
						}
						if (selected) { ImGui::SetItemDefaultFocus(); }
					}
					ImGui::EndCombo();
				}
				ImGui::PopID();
			}

			// Rays Per Pixel
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0); 
			DrawLabel("Rays per Pixel");
			ImGui::TableSetColumnIndex(1);
			ImGui::PushItemWidth(-FLT_MIN);
			if (ImGui::SliderInt("##EditorRPP", &editor.raysPerPixel, 1, 100)) {
				m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editor));
			}
			ImGui::TableSetColumnIndex(2);
			ImGui::PushItemWidth(-FLT_MIN);
			ImGui::SliderInt("##RuntimeRPP", &runtime.raysPerPixel, 1, 100);

			// Bounces Per Ray
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0); 
			DrawLabel("Bounces per Ray");
			ImGui::TableSetColumnIndex(1);
			ImGui::PushItemWidth(-FLT_MIN);
			if (ImGui::SliderInt("##EditorBounces", &editor.bouncesPerRay, 0, 100)) {
				m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editor));
			}
			ImGui::TableSetColumnIndex(2);
			ImGui::PushItemWidth(-FLT_MIN);
			ImGui::SliderInt("##RuntimeBounces", &runtime.bouncesPerRay, 0, 100);

			// ----------------------------
			// Misc
			// ----------------------------
			DrawSection("Misc");

			
			// Accumulate
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0); 
			DrawLabel("Accumulate Light");
			ImGui::TableSetColumnIndex(1);
			if (ImGui::Checkbox("##EditorAccumulate", &editor.accumulate)) {
				m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editor));
			}
			ImGui::TableSetColumnIndex(2);
			ImGui::Checkbox("##RuntimeAccumulate", &runtime.accumulate);


			// -----------------------------
			// Debug section
			// -----------------------------
			DrawSection("Debug");


			// BVH View
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0); 
			DrawLabel("BVH");
			ImGui::TableSetColumnIndex(1);
			if (ImGui::Checkbox("##EditorBVH", &editor.showBvhHeatmap)) {
				m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editor));
			}
			ImGui::TableSetColumnIndex(2); 
			ImGui::TextDisabled("-");

			if (editor.showBvhHeatmap) {
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0); 
				DrawLabel("BVH Color Cutoff");
				ImGui::TableSetColumnIndex(1);
				ImGui::PushItemWidth(-FLT_MIN);
				if (ImGui::SliderInt("##EditorBVHCutoff", &editor.bvhHeatmapColorCutoff, 1, 10000, "%d", ImGuiSliderFlags_Logarithmic)) {
					m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(editor));
				}
				ImGui::TableSetColumnIndex(2); ImGui::TextDisabled("-");
			}
			ImGui::EndTable();
		}
		ImGui::EndChild();

		if (m_EditorState->temp.isInRuntimeSimulation) {
			ImGui::EndDisabled();
		}

		ImGui::End();
	}
}