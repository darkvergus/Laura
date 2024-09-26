#include "EditorUI/SceneHierarchyPanel.h"
#include <IconsFontAwesome6.h>

namespace Laura
{

    void SceneHierarchyPanel::OnImGuiRender(std::shared_ptr<Scene> scene, std::shared_ptr<EditorState> editorState)
    {
        ImGui::Begin(ICON_FA_SITEMAP " Scene Hierarchy");

        if (ImGui::BeginMenu("Add"))
        {
            if (ImGui::MenuItem("Empty")) {}
            if (ImGui::BeginMenu("3D Object"))
            {
                if (ImGui::MenuItem("Sphere")) {}
                if (ImGui::MenuItem("Cuboid")) {}
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("2D Plane")) {}
            if (ImGui::MenuItem("Camera")) {}
            if (ImGui::MenuItem("3D Mesh")) {}

            ImGui::EndMenu();
        }

        entt::registry* activeRegistry = scene->Get();
        
        if(activeRegistry)
        {
            // Iterate over all ENTITIES in the scene
            auto view = activeRegistry->view<entt::entity>();
            for (auto entityID : view)
            {
                // Get the TAG of the entity (used to display in the hierarchy)
                Entity entity(entityID, activeRegistry);
                std::string& tag = entity.GetComponent<TagComponent>().Tag;
                
                // flags applied to every TreeNode
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow 
                                           | ImGuiTreeNodeFlags_OpenOnDoubleClick
                                           | ImGuiTreeNodeFlags_SpanAvailWidth;
                
                // if the entity is selected, highlight it
                if (entityID == editorState->selectedEntity)
                {
                    flags |= ImGuiTreeNodeFlags_Selected;
                }
                // display the treeNode in the scene hierarchy panel
                bool nodeOpen = ImGui::TreeNodeEx((void*)(uint64_t)entityID, flags, tag.c_str());
                
                if (ImGui::IsItemClicked())
                {
                    editorState->selectedEntity = entityID;
                }

                // display any children entities (if any)
                if (nodeOpen)
                {
                    // TODO: Child entity system
                    ImGui::BulletText("This is a testing text.");
                    ImGui::TreePop();
                }
            }

            // Deselect the selected entity if the user clicks in the window but outside of any tree node
            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				editorState->selectedEntity = entt::null;
			}
        }

        ImGui::End();

    }
}