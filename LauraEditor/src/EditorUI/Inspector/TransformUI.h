#pragma once
#include "Laura.h"
#include "EditorState.h"
#include <imgui_internal.h>

namespace Laura
{
	// Since the transform component can only get and set values through its own functions, we need to pass the set function as a lambda
	template <typename T>
	void TransformVec3Slider(	std::shared_ptr<EditorState> editorState, 
								const std::string& label, 
								glm::vec3 vector, 
								const T& setVector) {

		EditorTheme& theme = editorState->temp.editorTheme;

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
			theme.PushColor(ImGuiCol_Button, theme.SECONDARY_2);
			theme.PushColor(ImGuiCol_ButtonHovered, theme.SECONDARY_2);
			{
				theme.PushColor(ImGuiCol_ButtonActive, theme.X_COLOR);
				if (ImGui::Button("X", btnSize)) {
					setVector(glm::vec3(0.0f, vector.y, vector.z));
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##X", &vector.x, 0.1f)) {
					setVector(vector);
				}
				ImGui::SameLine();
				theme.PopColor(); // ButtonActive
				ImGui::PopItemWidth();

				ImGui::SetNextItemWidth(lineheight);
				theme.PushColor(ImGuiCol_ButtonActive, theme.Y_COLOR);
				if (ImGui::Button("Y", btnSize)) {
					setVector(glm::vec3(vector.x, 0.0f, vector.z));
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Y", &vector.y, 0.1f)) {
					setVector(vector);
				}
				ImGui::SameLine();
				theme.PopColor(); // ButtonActive
				ImGui::PopItemWidth();

				ImGui::SetNextItemWidth(lineheight);
				theme.PushColor(ImGuiCol_ButtonActive, theme.Z_COLOR);
				if (ImGui::Button("Z", btnSize)) {
					setVector(glm::vec3(vector.x, vector.y, 0.0f));
				}
				ImGui::SameLine();
				if (ImGui::DragFloat("##Z", &vector.z, 0.1f)) {
					setVector(vector);
				}
				ImGui::PopItemWidth();
			}
			theme.PopColor(2);
		}

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	inline void DrawTransformSliders(std::shared_ptr<EditorState> editorState, Entity entity) {
		auto& transform = entity.GetComponent<TransformComponent>();
		TransformVec3Slider(editorState, "Position", transform.GetTranslation(), [&transform](glm::vec3 vector) {
				transform.SetTranslation(vector); 
			}
		);

		TransformVec3Slider(editorState, "Rotation", transform.GetRotation(), [&transform](glm::vec3 vector) {
				transform.SetRotation(vector); 
			}
		);

		TransformVec3Slider(editorState, "Scale", transform.GetScale(), [&transform](glm::vec3 vector) {
				transform.SetScale(vector);
			}
		);
	}
}