#include "EditorLayer.h"
#include "imgui.h"

namespace Laura
{

	EditorLayer::EditorLayer(std::weak_ptr<IEventDispatcher> eventDispatcher,
							 std::shared_ptr<Asset::ResourcePool> resourcePool,
							 std::shared_ptr<Asset::Manager> assetManager,
							 std::shared_ptr<Profiler> profiler)
		:	m_EventDispatcher(eventDispatcher),
			m_ResourcePool(resourcePool),
			m_AssetManager(assetManager),
			m_Profiler(profiler),

			m_EditorState(std::make_shared<EditorState>()),
			m_InspectorPanel(m_EditorState),
			m_SceneHierarchyPanel(m_EditorState),
			m_ThemePanel(m_EditorState),
			m_ProfilerPanel(m_EditorState),
			m_RenderSettingsPanel(m_EditorState),
			m_AssetsPanel(m_EditorState, m_AssetManager, m_ResourcePool){
	}

	void EditorLayer::onAttach() {
		deserializeState(m_EditorState);
	}

	void EditorLayer::onDetach() {
		serializeState(m_EditorState);
	}

	void EditorLayer::onEvent(std::shared_ptr<IEvent> event) {
		std::cout << event->GetType() << std::endl;
		if (event->GetType() == EventType::SCENE_LOADED_EVENT) {
			m_Scene = std::dynamic_pointer_cast<SceneLoadedEvent>(event)->scene;
		}

		if (event->GetType() == EventType::NEW_FRAME_RENDERED_EVENT) {
			m_LatestFrameRender = std::dynamic_pointer_cast<NewFrameRenderedEvent>(event)->frame;
		}
	}

	void EditorLayer::DrawMainMenu() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::BeginMenu("Scene")) {
					if (ImGui::MenuItem("New")) {
						m_EventDispatcher->dispatchEvent(std::make_shared<SceneCreateEvent>());
					}
					if (ImGui::MenuItem("Close")) {
						m_EventDispatcher->dispatchEvent(std::make_shared<SceneCloseEvent>());
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {
				bool themePanelDisabled = m_EditorState->temp.isThemePanelOpen;
				if (themePanelDisabled)			 { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem("Themes"  )) { m_EditorState->temp.isThemePanelOpen = true; }
				if (themePanelDisabled)			 { ImGui::EndDisabled(); }
				bool profilerPanelDisabled = m_EditorState->temp.isProfilerPanelOpen;
				if (profilerPanelDisabled)		 { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem("Profiler")) { m_EditorState->temp.isProfilerPanelOpen = true; }
				if (profilerPanelDisabled)		 { ImGui::EndDisabled(); }
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	// main rendering function called every frame
	void EditorLayer::onImGuiRender() {
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		DrawMainMenu();

		bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);
		m_SceneHierarchyPanel.OnImGuiRender(m_Scene);
		m_InspectorPanel.OnImGuiRender(m_Scene);
		m_ThemePanel.OnImGuiRender();
		m_AssetsPanel.OnImGuiRender();
		m_RenderSettingsPanel.OnImGuiRender();
		m_ViewportPanel.OnImGuiRender(m_LatestFrameRender, m_EditorState);
		m_ProfilerPanel.OnImGuiRender(m_Profiler);
	}

	void EditorLayer::onUpdate() {
	}
}