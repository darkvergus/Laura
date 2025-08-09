#include "EditorLayer.h"
#include "imgui.h"
#include "EditorUI/UtilityUI.h"

namespace Laura
{

	EditorLayer::EditorLayer(std::shared_ptr<Profiler> profiler,
							 std::shared_ptr<IEventDispatcher> eventDispatcher,
							 std::shared_ptr<ProjectManager> projectManager,
							 std::shared_ptr<ImGuiContext> imGuiContext)
		:	m_EventDispatcher(eventDispatcher),
			m_ProjectManager(projectManager),
			m_Profiler(profiler),
			m_ImGuiContext(imGuiContext),

			m_EditorState(std::make_shared<EditorState>()),
			m_Launcher(m_EditorState, m_ProjectManager),
			m_InspectorPanel(m_EditorState, m_ProjectManager),
			m_SceneHierarchyPanel(m_EditorState, m_ProjectManager),
			m_ViewportPanel(m_EditorState, m_ProjectManager),
			m_ThemePanel(m_EditorState),
			m_ProfilerPanel(m_EditorState),
			m_RenderSettingsPanel(m_EditorState),
			m_AssetsPanel(m_EditorState, m_ProjectManager){
	}

	void EditorLayer::onAttach() {
		deserializeState(m_EditorState);
	}

	void EditorLayer::onDetach() {
		serializeState(m_EditorState);
	}

	void EditorLayer::onEvent(std::shared_ptr<IEvent> event) {
		//std::cout << event->GetType() << std::endl;

		if (event->GetType() == EventType::NEW_FRAME_RENDERED_EVENT) {
			m_LatestFrameRender = std::dynamic_pointer_cast<NewFrameRenderedEvent>(event)->frame;
		}
	}

	void EditorLayer::DrawMainMenu() {
		static bool shouldCloseProject = false;

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Project")) {
				if (ImGui::MenuItem("Save")) { m_ProjectManager->SaveProject(); }
				if (ImGui::MenuItem("Close")) { shouldCloseProject = true; }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window")) {
				bool themePanelDisabled = m_EditorState->temp.isThemePanelOpen;
				if (themePanelDisabled)			 { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem(ICON_FA_BRUSH " THEMES"  )) { m_EditorState->temp.isThemePanelOpen = true; }
				if (themePanelDisabled)			 { ImGui::EndDisabled(); }
				bool profilerPanelDisabled = m_EditorState->temp.isProfilerPanelOpen;
				if (profilerPanelDisabled)		 { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem(ICON_FA_STOPWATCH " PROFILER")) { m_EditorState->temp.isProfilerPanelOpen = true; }
				if (profilerPanelDisabled)		 { ImGui::EndDisabled(); }
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ConfirmAndExecute(shouldCloseProject, 
		"Close Project", 
		"Are you sure you want to close the project?\nMake sure you've saved your work to avoid losing changes.", 
		[&]() {
			m_ProjectManager->CloseProject();
		}, 
		m_EditorState);
	}

	void EditorLayer::onUpdate() {
		m_ImGuiContext->BeginFrame();
		m_EditorState->temp.editorTheme.ApplyAllToImgui(); // apply theme every frame

		if (!m_ProjectManager->ProjectIsOpen()) {
			m_Launcher.OnImGuiRender();
			m_ImGuiContext->EndFrame();
			return;
		}

		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		DrawMainMenu();
		bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);
		m_SceneHierarchyPanel.OnImGuiRender();
		m_InspectorPanel.OnImGuiRender();
		m_ThemePanel.OnImGuiRender();
		m_AssetsPanel.OnImGuiRender();
		m_RenderSettingsPanel.OnImGuiRender();
		m_ViewportPanel.OnImGuiRender(m_LatestFrameRender);
		m_ProfilerPanel.OnImGuiRender(m_Profiler);

		m_ImGuiContext->EndFrame();
	}
}