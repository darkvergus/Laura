#pragma once
#include "Laura.h"
#include "EditorUI/Themes/EditorTheme.h"
#include <imgui_internal.h>

namespace Laura
{
	// Since the transform component can only get and set values through its own functions, we need to pass the set function as a lambda
	template <typename T>
	void TransformVec3Slider(std::shared_ptr<ThemeManager> themeManager, const std::string& label, glm::vec3 vector, const T& setVector)
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Columns(2);
		// width of the 1st column (labels)
		ImGui::SetColumnWidth(0, 100.0f);
		float lineheight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 btnSize = ImVec2(lineheight * 0.7, lineheight);

		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::GetContentRegionAvail().x - 30);

		ImGui::PushID(label.c_str());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		{ // adding a scope for clarity
			ImGui::SetNextItemWidth(lineheight);
			themeManager->ImGuiSet(ImGuiCol_Button, themeManager->GetActiveTheme()->TransformButtonXbg);
			themeManager->ImGuiSet(ImGuiCol_ButtonHovered, themeManager->GetActiveTheme()->TransformButtonXHovered);
			themeManager->ImGuiSet(ImGuiCol_ButtonActive, themeManager->GetActiveTheme()->TransformButtonXActive);
			if (ImGui::Button("X", btnSize)) { setVector(glm::vec3(0.0f, vector.y, vector.z)); }
			ImGui::SameLine();
			if (ImGui::DragFloat("##X", &vector.x, 0.1f)) { setVector(vector); }
			ImGui::SameLine(); ImGui::PopItemWidth();

			ImGui::SetNextItemWidth(lineheight);
			themeManager->ImGuiSet(ImGuiCol_Button, themeManager->GetActiveTheme()->TransformButtonYbg);
			themeManager->ImGuiSet(ImGuiCol_ButtonHovered, themeManager->GetActiveTheme()->TransformButtonYHovered);
			themeManager->ImGuiSet(ImGuiCol_ButtonActive, themeManager->GetActiveTheme()->TransformButtonYActive);
			if (ImGui::Button("Y", btnSize)) { setVector(glm::vec3(vector.x, 0.0f, vector.z)); }
			ImGui::SameLine();
			if (ImGui::DragFloat("##Y", &vector.y, 0.1f)) { setVector(vector); }
			ImGui::SameLine(); ImGui::PopItemWidth();

			ImGui::SetNextItemWidth(lineheight);
			themeManager->ImGuiSet(ImGuiCol_Button, themeManager->GetActiveTheme()->TransformButtonZbg);
			themeManager->ImGuiSet(ImGuiCol_ButtonHovered, themeManager->GetActiveTheme()->TransformButtonZHovered);
			themeManager->ImGuiSet(ImGuiCol_ButtonActive, themeManager->GetActiveTheme()->TransformButtonZActive);
			if (ImGui::Button("Z", btnSize)) { setVector(glm::vec3(vector.x, vector.y, 0.0f)); }
			ImGui::SameLine();
			if (ImGui::DragFloat("##Z", &vector.z, 0.1f)) { setVector(vector); }
			ImGui::PopItemWidth();
		}
		ImGui::PopStyleVar();
		ImGui::PopID();
		themeManager->ImGuiSet(ImGuiCol_Button, themeManager->GetActiveTheme()->DefaultButton);
		themeManager->ImGuiSet(ImGuiCol_ButtonActive, themeManager->GetActiveTheme()->DefaultButtonHovered);
		themeManager->ImGuiSet(ImGuiCol_ButtonHovered, themeManager->GetActiveTheme()->DefaultButtonActive);
		ImGui::Columns(1);
	}

	inline void DrawTransformSliders(std::shared_ptr<ThemeManager> themeManager, Entity entity)
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		TransformVec3Slider(themeManager, "Position", transform.GetTranslation(), [&transform](glm::vec3 vector) {transform.SetTranslation(vector); });
		TransformVec3Slider(themeManager, "Rotation", transform.GetRotation(), [&transform](glm::vec3 vector) {transform.SetRotation(vector); });
		if (entity.HasComponent<CameraComponent>()) { ImGui::BeginDisabled(); }
		TransformVec3Slider(themeManager, "Scale", transform.GetScale(), [&transform](glm::vec3 vector) {transform.SetScale(vector); });
		if (entity.HasComponent<CameraComponent>()) { ImGui::EndDisabled(); }
	}
}