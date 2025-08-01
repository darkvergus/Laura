#include "EditorLayer.h"
#include "imgui.h"
#include "EditorUI/UtilityUI.h"

namespace Laura
{

	EditorLayer::EditorLayer(std::weak_ptr<IEventDispatcher> eventDispatcher,
							 std::shared_ptr<ImGuiContext> imGuiContext,
							 std::shared_ptr<AssetPool> resourcePool,
							 std::shared_ptr<AssetManager> assetManager,
							 std::shared_ptr<Profiler> profiler)
		:	m_EventDispatcher(eventDispatcher),
			m_AssetPool(resourcePool),
			m_AssetManager(assetManager),
			m_Profiler(profiler),

			m_EditorState(std::make_shared<EditorState>()),
			m_ImGuiContext(imGuiContext),

			m_Launcher(m_EditorState),
			m_InspectorPanel(m_EditorState),
			m_SceneHierarchyPanel(m_EditorState),
			m_ThemePanel(m_EditorState),
			m_ProfilerPanel(m_EditorState),
			m_RenderSettingsPanel(m_EditorState),
			m_AssetsPanel(m_EditorState, m_AssetManager, m_AssetPool){
	}

	void EditorLayer::onAttach() {
		deserializeState(m_EditorState);
	}

	void EditorLayer::onDetach() {
		serializeState(m_EditorState);
	}

	void EditorLayer::onEvent(std::shared_ptr<IEvent> event) {
		if (event->GetType() != EventType::NEW_FRAME_RENDERED_EVENT) {
			std::cout << event->GetType() << std::endl;
		}

		if (event->GetType() == EventType::SCENE_LOADED_EVENT) {
			m_Scene = std::dynamic_pointer_cast<SceneLoadedEvent>(event)->scene;
		}

		if (event->GetType() == EventType::NEW_FRAME_RENDERED_EVENT) {
			m_LatestFrameRender = std::dynamic_pointer_cast<NewFrameRenderedEvent>(event)->frame;
		}
	}

	void EditorLayer::DrawMainMenu() {
		static bool shouldCloseScene = false;
		static bool shouldOpenScene = false;
		static bool shouldSaveScene = false;
		static bool shouldNewScene = false;

		if (ImGui::BeginMainMenuBar()) {
			// FILE TAB
			if (ImGui::BeginMenu("File")) {
				if (ImGui::BeginMenu("Scene")) {
					if (ImGui::MenuItem("New")) { shouldNewScene = true; }
					if (ImGui::MenuItem("Open")) { shouldOpenScene = true; }
					if (ImGui::MenuItem("Save")) { shouldSaveScene = true; }
					if (ImGui::MenuItem("Close")) { shouldCloseScene = true; }
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			// VIEW TAB
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

		// FILE > SCENE > NEW/OPEN/SAVE/CLOSE
		if (shouldNewScene) {
			m_EventDispatcher->dispatchEvent(std::make_shared<SceneCreateEvent>());
			shouldNewScene = false;
		}

		if (shouldOpenScene) {
			OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
			char buff[MAX_PATH] = {};
			ofn.lpstrFilter = SCENE_FILE_EXTENSION " Files\0 * " SCENE_FILE_EXTENSION "\0";
			ofn.lpstrTitle = "Select Scene:";
			ofn.nMaxFile = sizeof(buff);
			ofn.lpstrFile = buff;
			if (GetOpenFileNameA(&ofn)) {
				std::filesystem::path openFilepath(buff);
				m_EventDispatcher->dispatchEvent(std::make_shared<SceneOpenEvent>(openFilepath));
			}
			shouldOpenScene = false;
		}

		if (shouldSaveScene) {
			OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
			char buff[MAX_PATH] = "template" SCENE_FILE_EXTENSION;
			ofn.lpstrFilter = SCENE_FILE_EXTENSION " Files\0 * " SCENE_FILE_EXTENSION "\0";
			ofn.lpstrTitle = "Save Scene:";
			ofn.nMaxFile = sizeof(buff);
			ofn.lpstrFile = buff;
			ofn.Flags = OFN_OVERWRITEPROMPT;
			if (GetSaveFileNameA(&ofn)) {
				std::filesystem::path saveFilepath(buff);
				saveFilepath.replace_extension(SCENE_FILE_EXTENSION);
				m_EventDispatcher->dispatchEvent(std::make_shared<SceneSaveEvent>(saveFilepath));
			}
			shouldSaveScene = false;
		}

		ConfirmAndExecute(shouldCloseScene,
			"Close Scene",
			"Are you sure you want to close the scene? (If not saved changes will be lost)",
			[&]() {
				m_EventDispatcher->dispatchEvent(std::make_shared<SceneCloseEvent>());
			},
			m_EditorState
		);
	}

	void EditorLayer::onUpdate() {
		m_ImGuiContext->BeginFrame();
		std::filesystem::path projectRoot = "";
		if (projectRoot.string() != "") {
			bool showDemo = true;
			ImGui::ShowDemoWindow(&showDemo);
			m_Launcher.OnImGuiRender();
		}
		else {
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
		m_ImGuiContext->EndFrame();
	}
}