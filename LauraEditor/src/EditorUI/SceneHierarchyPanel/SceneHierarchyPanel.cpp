#include "EditorUI/SceneHierarchyPanel/SceneHierarchyPanel.h"
#include "EditorUI/UtilityUI.h"
#include <IconsFontAwesome6.h>

namespace Laura
{
    SceneHierarchyPanel::SceneHierarchyPanel(std::shared_ptr<EditorState> editorState)
		: m_EditorState(editorState)
    {
    }

    void SceneHierarchyPanel::OnImGuiRender(std::shared_ptr<Scene> scene) {
        EditorTheme& theme = m_EditorState->temp.editorTheme;

        ImGui::Begin(ICON_FA_SITEMAP " Scene Hierarchy");

        if (ImGui::BeginMenu(ICON_FA_SQUARE_PLUS " Add")) {
            if (ImGui::MenuItem("Empty")) {
                scene->CreateEntity();
            }
            ImGui::EndMenu();
        }

        entt::registry* activeRegistry = scene->GetRegistry();
        if(!activeRegistry) {
			ImGui::End();
            LOG_ENGINE_WARN("SceneHierarchyPanel::OnImGuiRender: activeRegistry is nullptr.");
			return;
		}
        
        ImVec2 panelDims = ImGui::GetContentRegionAvail();
        float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
                                    | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap
                                    | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

        static bool destroyEntity = false; // since you can only delete one entity at a time the fact that this is used in the loop is fine
        /// ITERATE OVER ENTITIES /////////////////////////////////////////////////////////////
        auto view = activeRegistry->view<entt::entity>();
        for (auto entityID : view) {
            bool entityChildrenOpen = false;
            Entity entity(entityID, activeRegistry);
            std::string& tag = entity.GetComponent<TagComponent>().Tag;

            // display selected entity
            if (entityID == m_EditorState->temp.selectedEntity) {
                theme.PushColor(ImGuiCol_Header, EditorCol_Secondary2);

                entityChildrenOpen = ImGui::TreeNodeEx((void*)(uint64_t)entityID, flags, tag.c_str());
                ImGui::SameLine(panelDims.x - lineHeight * 0.5);
                if (ImGui::Button(ICON_FA_TRASH_CAN)) { destroyEntity = true; }
                ConfirmAndExecute(destroyEntity, ICON_FA_TRASH_CAN " Delete Entity", "Are you sure you want to delete this entity?", [&]() {
                        scene->DestroyEntity(entity);
                        m_EditorState->temp.selectedEntity = entt::null;
                    }, m_EditorState);
                theme.PopColor();
			}
			else { // display non-selected entity
				entityChildrenOpen = ImGui::TreeNodeEx((void*)(uint64_t)entityID, flags, tag.c_str());
			}

            if (ImGui::IsItemClicked()) {
                m_EditorState->temp.selectedEntity = entityID;
            }

            if (entityChildrenOpen) {
                // TODO: Child entity system
                ImGui::BulletText("This is a testing text.");
                ImGui::TreePop();
            }
        }

        // Deselect the selected entity if the user clicks in the window but outside of any tree node
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_EditorState->temp.selectedEntity = entt::null;
		}

        ImGui::PopStyleVar();
        ImGui::End();
    }
}