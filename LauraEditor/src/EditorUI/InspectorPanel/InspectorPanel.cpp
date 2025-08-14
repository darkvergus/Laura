#include "EditorUI/InspectorPanel/InspectorPanel.h"
#include "EditorUI/InspectorPanel/TransformUI.h"
#include "EditorUI/UtilityUI.h"

namespace Laura
{

	/// INSPECTOR PANEL METHODS //////////////////////////////////////////////////////////////
	InspectorPanel::InspectorPanel(std::shared_ptr<EditorState> editorState, std::shared_ptr<ProjectManager> projectManager)
		: m_EditorState(editorState), m_ProjectManager(projectManager) {
	}

    void InspectorPanel::OnImGuiRender() {
		EditorTheme& theme = m_EditorState->temp.editorTheme;
		
		
		ImGui::SetNextWindowSizeConstraints({ 350, 50 }, {FLT_MAX, FLT_MAX});
		ImGui::Begin(ICON_FA_CIRCLE_INFO " INSPECTOR");
		if (m_EditorState->temp.isInRuntimeMode) {
			ImGui::BeginDisabled();
		}

        if (!m_ProjectManager->ProjectIsOpen()) {
			if (m_EditorState->temp.isInRuntimeMode) {
				ImGui::EndDisabled();
			}
            ImGui::End();
            return;
        }

        std::shared_ptr<Scene> scene = m_ProjectManager->GetSceneManager()->GetOpenScene();

        if (scene == nullptr || m_EditorState->temp.selectedEntity == entt::null) {
			if (m_EditorState->temp.isInRuntimeMode) {
				ImGui::EndDisabled();
			}
			ImGui::End();
			return;
		}

		theme.PushColor(ImGuiCol_FrameBg, EditorCol_Primary2);
        entt::registry* activeRegistry = scene->GetRegistry();
        entt::entity selectedEntity = m_EditorState->temp.selectedEntity;
        EntityHandle entity(selectedEntity, activeRegistry);
		
		// TAG COMPONENT
        if (entity.HasComponent<TagComponent>()) {
			theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());
			ImGui::AlignTextToFramePadding();
			ImGui::Text(ICON_FA_TAG " Name:"); ImGui::SameLine();
			theme.PopColor();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::InputTextWithHint("##Tag Component", "Entity Name", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
			ImGui::Dummy({ 0.0f, 10.0f });
		}

		// TRANSFORM COMPONENT
		DrawComponent<TransformComponent>(std::string(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT " Transform"), entity, [&](EntityHandle& _entity) {
				DrawTransformSliders(m_EditorState, _entity);
			}
		);

		// CAMERA COMPOENENT
		DrawComponent<CameraComponent>(std::string(ICON_FA_VIDEO " Camera Component"), entity, [&](EntityHandle& entity) {
				auto& cameraComponent = entity.GetComponent<CameraComponent>();

				theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
				ImGui::Text("Main Camera:");
				theme.PopColor();
				ImGui::SameLine();
				theme.PushColor(ImGuiCol_CheckMark, EditorCol_Text1);
				if (ImGui::Checkbox("##MainCameraCheckbox", &cameraComponent.isMain)) {
					for (auto e : scene->GetRegistry()->view<CameraComponent>()) {
						EntityHandle otherEntity(e, scene->GetRegistry());
						if (otherEntity.GetComponent<GUIDComponent>().guid != entity.GetComponent<GUIDComponent>().guid) {
							otherEntity.GetComponent<CameraComponent>().isMain = false;
						}
					}
				}
				theme.PopColor();

				theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
				ImGui::Text("FOV");
				theme.PopColor();
				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::DragFloat("##fovDragInt", &cameraComponent.fov, 0.1f, 10.0f, 130.0f, "%.1f");
			}
		);

		// MESH COMPONENT
		DrawComponent<MeshComponent>(std::string(ICON_FA_CUBE " Mesh"), entity, [&theme](EntityHandle& entity) {
				std::string& sourceName = entity.GetComponent<MeshComponent>().sourceName;
				ImGui::Dummy({ 0.0f, 5.0f });
				theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
				ImGui::Text("Mesh:");
				theme.PopColor();

				ImGui::SameLine();
				theme.PushColor(ImGuiCol_Header, EditorCol_Secondary2);
				ImGui::Selectable((sourceName + "##MeshSelectable").c_str(), true);
				theme.PopColor();
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DNDPayloadTypes::MESH)) {
						IM_ASSERT(payload->DataSize == sizeof(DNDPayload));
						auto& meshPayload = *static_cast<DNDPayload*>(payload->Data);
						sourceName = meshPayload.title; // copy char title[256] into std::string
						entity.GetComponent<MeshComponent>().guid = meshPayload.guid;
                    }
                    ImGui::EndDragDropTarget();
                }
			}
		);

		DrawComponent<MaterialComponent>(std::string(ICON_FA_LAYER_GROUP " Material"), entity, [&](EntityHandle& entity) {
				auto& materialComponent = entity.GetComponent<MaterialComponent>();
				ImGui::Dummy({ 0.0f, 5.0f });

				theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
				ImGui::Text("Emission Strength:");
				theme.PopColor();
				ImGui::SameLine(150.0f);
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::SliderFloat("##emission strength", &materialComponent.emission.w, 0.0f, 1.0f);

				theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
				ImGui::Text("Emission Color:");
				theme.PopColor();
				ImGui::SameLine(150.0f);
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::ColorEdit3("##emission color", glm::value_ptr(materialComponent.emission), ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoInputs);

				theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
				ImGui::Text("Color:");
				theme.PopColor();
				ImGui::SameLine(150.0f);
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::ColorEdit3("##color", glm::value_ptr(materialComponent.color), ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoInputs);
			}
		);

		// ADD COMPONENT BUTTON
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::Separator();
		ImVec2 panelDims = ImGui::GetContentRegionAvail();
		float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::SetCursorPosX(panelDims.x / 6);
		bool popupOpened = false;
		
		theme.PushColor(ImGuiCol_Button, EditorCol_Secondary2);
		float buttonWidth = panelDims.x * (2.0f / 3.0f);
		if (ImGui::Button("Add Component", { buttonWidth, lineHeight })) {
			popupOpened = true;
			ImGui::OpenPopup("AddComponent");
		}
		theme.PopColor();

		if (ImGui::IsPopupOpen("AddComponent")) {
			ImVec2 addButtonPos = ImGui::GetItemRectMin();
			ImVec2 addButtonSize = ImGui::GetItemRectSize();
			ImGui::SetNextWindowSizeConstraints(
				ImVec2(FLT_MIN, FLT_MIN),
				ImVec2(FLT_MAX, 300.0f) // if the popup contains too many compnents, adds a scrollbar
			);
			ImGui::SetNextWindowPos(ImVec2(addButtonPos.x, addButtonPos.y + addButtonSize.y));
			ImGui::SetNextWindowSize(ImVec2(buttonWidth, 0.0f));
		}

		if (ImGui::BeginPopup("AddComponent")) {
			GiveEntityComponentButton<TransformComponent>	(entity, "Transform", ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT);
			GiveEntityComponentButton<CameraComponent>		(entity, "Camera", ICON_FA_VIDEO);
			GiveEntityComponentButton<MeshComponent>		(entity, "Mesh", ICON_FA_CUBE);
			GiveEntityComponentButton<MaterialComponent>	(entity, "Material", ICON_FA_LAYER_GROUP);
			ImGui::EndPopup();
		}

		// ensure that there is always some space under the Add Component button when scrolling to display the popup
		ImGui::Dummy(ImVec2(0.0f, 100.0f)); 
		theme.PopColor();
		
		if (m_EditorState->temp.isInRuntimeMode) {
			ImGui::EndDisabled();
		}
		
        ImGui::End();
    }
}