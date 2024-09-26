#include "EditorUI/Inspector/InspectorPanel.h"
#include <IconsFontAwesome6.h>
#include <imgui_internal.h>
#include "EditorUI/Inspector/TransformUI.h"

namespace Laura
{
	template<typename T, typename UILambda>
	inline void InspectorPanel::DrawComponent(const std::string& TreenodeTitle, Entity entity, UILambda uiLambda, bool removable)
	{
		const ImGuiTreeNodeFlags treenodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap 
											   | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			// unique id for each component (entity id + component type)
			std::string idStr = std::to_string((uint64_t)entity.GetID()) + TreenodeTitle;
			ImGui::PushID(idStr.c_str());

			ImVec2 panelDims = ImGui::GetContentRegionAvail();
			float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
			bool open = ImGui::TreeNodeEx(TreenodeTitle.c_str(), treenodeFlags);
			ImGui::SameLine(panelDims.x - 1.5*lineHeight - 0.2);

			if (removable)
			{
				if (!m_EditorState->doubleConfirmDelete)
				{
					if (ImGui::Button(ICON_FA_TRASH_CAN))
					{
						entity.RemoveComponent<T>();
					}
				}
				else
				{
					if (ImGui::Button(ICON_FA_TRASH_CAN))
					{
						ImGui::OpenPopup(ICON_FA_TRASH_CAN " Delete Component");
					}
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
					if (ImGui::BeginPopupModal(ICON_FA_TRASH_CAN " Delete Component", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("Are you sure you want to delete this component?");

						float panelWidth = ImGui::GetContentRegionAvail().x;
						float buttonWidth = panelWidth * 0.5f - 5.0f;

						ImGui::Dummy({ 5.0f, 0.0f });
						m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->ButtonGray);
						if (ImGui::Button("Yes", ImVec2(buttonWidth, 0)))
						{
							entity.RemoveComponent<T>();
							ImGui::CloseCurrentPopup();
						}
						ImGui::SameLine();
						if (ImGui::Button("No", ImVec2(buttonWidth, 0)))
						{
							ImGui::CloseCurrentPopup();
						}
						m_ThemeManager->ImGuiSet(ImGuiCol_Button, m_ThemeManager->GetActiveTheme()->DefaultButton);
						ImGui::EndPopup();
					}
					ImGui::PopStyleVar();
				}	
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

			if (open)
			{
				uiLambda(entity);
				ImGui::TreePop();
				ImGui::Spacing();
			}

			ImGui::PopID();
		}
	}

	/// INSPECTOR PANEL METHODS //////////////////////////////////////////////////////////////
	InspectorPanel::InspectorPanel(std::shared_ptr<EditorState> editorState, std::shared_ptr<ThemeManager> themeManager)
		: m_EditorState(editorState), m_ThemeManager(themeManager)
	{
	}

    void InspectorPanel::OnImGuiRender(std::shared_ptr<Scene> scene)
    {
		ImGui::SetNextWindowSizeConstraints({ 350, 50 }, {FLT_MAX, FLT_MAX});
        if (m_EditorState->selectedEntity == entt::null)
		{
			ImGui::Begin(ICON_FA_CIRCLE_INFO " Inspector");
			ImGui::End();
			return;
		}

        entt::registry* activeRegistry = scene->Get();
        entt::entity selectedEntity = m_EditorState->selectedEntity;
        Entity entity(selectedEntity, activeRegistry); // convert to Laura Entity

        ImGui::Begin(ICON_FA_CIRCLE_INFO " Inspector");

		/// TAG COMPONENT UI //////////////////////////////////////////////////////////////////
        if (entity.HasComponent<TagComponent>())
		{
			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());
			ImGui::AlignTextToFramePadding();
			ImGui::Text(ICON_FA_TAG); ImGui::SameLine();
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
			ImGui::Separator();
			ImGui::Spacing();
		}

		/// CAMERA COMPOENENT UI //////////////////////////////////////////////////////////////
		DrawComponent<CameraComponent>(std::string(ICON_FA_VIDEO " Camera Component"), entity, [](Entity& entity)
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				ImGui::Checkbox("Main", &cameraComponent.isMain);
				ImGui::DragFloat("FOV", &cameraComponent.fov, 0.1f, 10.0f, 130.0f, "%.1f");
			});

		/// TRANSFORM COMPONENT UI ////////////////////////////////////////////////////////////
		DrawComponent<TransformComponent>(std::string(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT " Transform"), entity, [&](Entity& _entity)
			{
				DrawTransformSliders(m_ThemeManager, _entity);
			});

		/// MESH COMPONENT UI /////////////////////////////////////////////////////////////////

		DrawComponent<MeshComponent>(std::string(ICON_FA_CUBE " Mesh"), entity, [](Entity& entity)
			{
				auto& meshComponent = entity.GetComponent<MeshComponent>();
				ImGui::Text("Mesh ID: %d", meshComponent.GetID());
			});

		/// MATERIAL COMPONENT UI /////////////////////////////////////////////////////////////
		// TODO
        ImGui::End();
    }
}