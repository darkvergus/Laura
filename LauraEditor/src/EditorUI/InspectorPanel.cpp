#include "EditorUI/InspectorPanel.h"
#include <IconsFontAwesome6.h>

namespace Laura
{

    void InspectorPanel::OnImGuiRender(std::shared_ptr<Scene> scene, EditorState* editorState)
    {
        if (editorState->selectedEntity == entt::null)
		{
			ImGui::Begin("Inspector");
			ImGui::Text("Select an entity to inspect");
			ImGui::End();
			return;
		}

        entt::registry* activeRegistry = scene->Get();
        entt::entity selectedEntity = editorState->selectedEntity;
        Entity entity(selectedEntity, activeRegistry); // convert to Laura Entity

        ImGui::Begin("Inspector");

		/// TAG COMPONENT UI //////////////////////////////////////////////////////////////////

        if (entity.HasComponent<TagComponent>())
		{
			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());
			if (ImGui::InputText("Tag Component", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		/// CAMERA COMPOENENT UI //////////////////////////////////////////////////////////////
		bool isCamera = entity.HasComponent<CameraComponent>();
		if (isCamera)
		{
			ImGui::SeparatorText("Camera Component");
			CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();
			ImGui::Checkbox("Main", &cameraComponent.isMain);
			ImGui::DragFloat("FOV", &cameraComponent.fov, 0.1f, 10.0f, 130.0f, "%.1f");
		}

		/// TRANSFORM COMPONENT UI ////////////////////////////////////////////////////////////
		
		if (entity.HasComponent<TransformComponent>())
		{
			TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();

			glm::vec3 position = transformComponent.GetTranslation();
			glm::vec3 rotation = transformComponent.GetRotation();
			glm::vec3 scale = transformComponent.GetScale();

			ImGui::SeparatorText("Transform Component");

			ImGui::Text("Position");
			float sliders_width = 200;
			ImGui::PushItemWidth(sliders_width / 3.0f);
			ImGui::PushID("Position");
			
			if (ImGui::DragFloat("X", &position.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { transformComponent.SetTranslation(position); }
			ImGui::SameLine(0, 10);
			if (ImGui::DragFloat("Y", &position.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { transformComponent.SetTranslation(position); }
			ImGui::SameLine(0, 10);
			if (ImGui::DragFloat("Z", &position.z, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { transformComponent.SetTranslation(position); }
			
			ImGui::PopItemWidth();
			ImGui::PopID();

			ImGui::Text("Rotation");
			ImGui::PushItemWidth(sliders_width / 3.0f);
			ImGui::PushID("Rotation");
			
			if (ImGui::DragFloat("X", &rotation.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { transformComponent.SetRotation(rotation); }
			ImGui::SameLine(0, 10);
			if (ImGui::DragFloat("Y", &rotation.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { transformComponent.SetRotation(rotation); }
			ImGui::SameLine(0, 10);
			if (ImGui::DragFloat("Z", &rotation.z, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { transformComponent.SetRotation(rotation); }
			
			ImGui::PopItemWidth();
			ImGui::PopID();			

			ImGui::Text("Scale");
			ImGui::PushItemWidth(sliders_width / 3.0f);
			ImGui::PushID("Scale");

			if (isCamera){ ImGui::BeginDisabled(); }
			if (ImGui::DragFloat("X", &scale.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { transformComponent.SetScale(scale); }
			ImGui::SameLine(0, 10);
			if (ImGui::DragFloat("Y", &scale.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { transformComponent.SetScale(scale); }
			ImGui::SameLine(0, 10);
			if (ImGui::DragFloat("Z", &scale.z, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { transformComponent.SetScale(scale); }
			if (isCamera){ ImGui::EndDisabled(); }
			ImGui::PopItemWidth();
			ImGui::PopID();
		}

		/// MESH COMPONENT UI /////////////////////////////////////////////////////////////////
		if (entity.HasComponent<MeshComponent>())
		{
			MeshComponent& meshComponent = entity.GetComponent<MeshComponent>();
			ImGui::SeparatorText("Mesh Component");		
		}

		/// MATERIAL COMPONENT UI /////////////////////////////////////////////////////////////


        ImGui::End();
    }
}