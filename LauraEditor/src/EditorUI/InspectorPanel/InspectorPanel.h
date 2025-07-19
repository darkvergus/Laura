#pragma once 
#include <IconsFontAwesome6.h>
#include <imgui_internal.h>

#include "Laura.h"
#include "EditorState.h"
#include "EditorTheme.h"
#include "EditorUI/UtilityUI.h"
#include "EditorUI/DNDPayloads.h"

namespace Laura
{
	class InspectorPanel
	{
	public:
		InspectorPanel(std::shared_ptr<EditorState> editorState);
		~InspectorPanel() = default;

		void OnImGuiRender(std::shared_ptr<Scene> scene);

	private:
		template<typename T, typename UILambda>
		inline void DrawComponent(const std::string& TreenodeTitle, Entity& entity, UILambda uiLambda, bool removable = true)
		{
			const ImGuiTreeNodeFlags treenodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap
													| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;
			static bool deleteComponent = false;
			if (entity.HasComponent<T>())
			{
				ImVec2 panelDims = ImGui::GetContentRegionAvail();
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

				// unique id for each component (entity id + component type)
				std::string idStr = std::to_string((uint64_t)entity.GetID()) + TreenodeTitle;

				ImGui::PushID(idStr.c_str());
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));

				bool componentTreeNodeOpen = ImGui::TreeNodeEx(TreenodeTitle.c_str(), treenodeFlags);
				ImGui::SameLine(panelDims.x - 1.5 * lineHeight - 0.2);

				if (removable)
				{
					if (ImGui::Button(ICON_FA_TRASH_CAN))
					{
						deleteComponent = true;
						ImGui::OpenPopup(ICON_FA_TRASH_CAN " Delete Component");
					}

					ConfirmAndExecute(deleteComponent, ICON_FA_TRASH_CAN " Delete Component", "Are you sure you want to delete this component?", [&]() {
						entity.RemoveComponent<T>();
					}, m_EditorState);
				}

				ImGui::SameLine(panelDims.x - lineHeight * 0.5);
				if (ImGui::Button(ICON_FA_GEARS))
				{
					ImGui::OpenPopup("ComponentSettings");
				}
				ImGui::PopStyleVar();

				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Reset"))
					{
						entity.RemoveComponent<T>();
						entity.AddComponent<T>();
					}
					ImGui::EndPopup();
				}

				if (componentTreeNodeOpen)
				{
					if (entity.HasComponent<T>()) // the component could have been removed in the meantime
					{
						uiLambda(entity);
					}
					ImGui::TreePop();
					ImGui::Spacing();
				}
				ImGui::PopID();
			}
		}

        template<typename T>
        void GiveEntityComponentButton(Entity entity, const char* label, const char* icon) {
			if (entity.HasComponent<T>()) { ImGui::BeginDisabled(); }

			if (ImGui::Selectable((icon + std::string(" ") + label).c_str(), false)) { 
				entity.AddComponent<T>(); 
				return; // avoid calling EndDisabled()
			}

            if (entity.HasComponent<T>()) { ImGui::EndDisabled(); }
        }

	private:
		std::shared_ptr<EditorState> m_EditorState;
	};
}