#include "EditorUI/Inspector/InspectorPanel.h"
#include "EditorUI/Inspector/TransformUI.h"


namespace Laura
{
	/// INSPECTOR PANEL METHODS //////////////////////////////////////////////////////////////
	InspectorPanel::InspectorPanel(std::shared_ptr<EditorState> editorState)
		: m_EditorState(editorState) {
	}

    void InspectorPanel::OnImGuiRender(std::shared_ptr<Scene> scene) {
		EditorTheme& theme = m_EditorState->temp.editorTheme;

		ImGui::SetNextWindowSizeConstraints({ 350, 50 }, {FLT_MAX, FLT_MAX});
        if (m_EditorState->temp.selectedEntity == entt::null) {
			ImGui::Begin(ICON_FA_CIRCLE_INFO " Inspector");
			ImGui::End();
			return;
		}

        entt::registry* activeRegistry = scene->GetRegistry();
        entt::entity selectedEntity = m_EditorState->temp.selectedEntity;
        Entity entity(selectedEntity, activeRegistry);

        ImGui::Begin(ICON_FA_CIRCLE_INFO " Inspector");

		/// TAG COMPONENT UI //////////////////////////////////////////////////////////////////
        if (entity.HasComponent<TagComponent>()) {
			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());
			ImGui::AlignTextToFramePadding();
			ImGui::Text(ICON_FA_TAG); ImGui::SameLine();
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}
			ImGui::Separator();
			ImGui::Spacing();
		}

		/// TRANSFORM COMPONENT UI ////////////////////////////////////////////////////////////
		DrawComponent<TransformComponent>(std::string(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT " Transform"), entity, [&](Entity& _entity) {
				DrawTransformSliders(m_EditorState, _entity);
			}
		);

		/// CAMERA COMPOENENT UI //////////////////////////////////////////////////////////////
		DrawComponent<CameraComponent>(std::string(ICON_FA_VIDEO " Camera Component"), entity, [&](Entity& entity) {
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				if (ImGui::Checkbox("Main", &cameraComponent.isMain)) {
					for (auto e : scene->GetRegistry()->view<CameraComponent>()) {
						Entity otherEntity(e, scene->GetRegistry());
						if (otherEntity.GetComponent<GUIDComponent>().guid != entity.GetComponent<GUIDComponent>().guid) {
							otherEntity.GetComponent<CameraComponent>().isMain = false;
						}
					}
				}
				ImGui::DragFloat("FOV", &cameraComponent.fov, 0.1f, 10.0f, 130.0f, "%.1f");
			}
		);

		/// MESH COMPONENT UI /////////////////////////////////////////////////////////////////
		DrawComponent<MeshComponent>(std::string(ICON_FA_CUBE " Mesh"), entity, [](Entity& entity) {
				auto& meshComponent = entity.GetComponent<MeshComponent>();
				ImGui::Text("Mesh ID: %d", meshComponent.guid);
			}
		);


		/// ADD COMPONENT BUTTON /////////////////////////
		ImVec2 panelDims = ImGui::GetContentRegionAvail();
		float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SetCursorPosX(panelDims.x / 6);
		bool popupOpened = false;
		
		theme.PushColor(ImGuiCol_Button, theme.SECONDARY_2);
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
        ImGui::End();
    }
}