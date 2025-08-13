#include <imgui.h>
#include "EditorLayer.h"
#include "EditorUI/UtilityUI.h"
#include "EditorUI/MainMenuPanel/MainMenuPanel.h"
#include "EditorUI/ViewportPanel/ViewportPanel.h"
#include "EditorUI/SceneHierarchyPanel/SceneHierarchyPanel.h"
#include "EditorUI/InspectorPanel/InspectorPanel.h"
#include "EditorUI/ProfilerPanel/ProfilerPanel.h"
#include "EditorUI/RenderSettingsPanel/RenderSettingsPanel.h"
#include "EditorUI/ThemePanel/ThemePanel.h"
#include "EditorUI/AssetsPanel/AssetsPanel.h"

namespace Laura
{

	EditorLayer::EditorLayer(std::shared_ptr<Profiler> profiler,
							 std::shared_ptr<IEventDispatcher> eventDispatcher,
							 std::shared_ptr<ProjectManager> projectManager,
							 std::shared_ptr<ImGuiContext> imGuiContext)
		: m_Profiler(profiler)
		, m_EventDispatcher(eventDispatcher)
		, m_ProjectManager(projectManager)
		, m_EditorState(std::make_shared<EditorState>())
		, m_ImGuiContext(imGuiContext)
		, m_Launcher(m_EditorState, m_ProjectManager)
		, m_EditorPanels({
			std::make_unique<MainMenuPanel>(m_EditorState, m_ProjectManager),
			std::make_unique<InspectorPanel>(m_EditorState, m_ProjectManager),
			std::make_unique<SceneHierarchyPanel>(m_EditorState, m_ProjectManager),
			std::make_unique<ViewportPanel>(m_EditorState, m_ProjectManager),
			std::make_unique<ThemePanel>(m_EditorState),
			std::make_unique<ProfilerPanel>(m_EditorState, m_Profiler),
			std::make_unique<RenderSettingsPanel>(m_EditorState, m_EventDispatcher),
			std::make_unique<AssetsPanel>(m_EditorState, m_ProjectManager)
		}){
	}

	void EditorLayer::onAttach() {
		deserializeState(m_EditorState);

		for (auto& panel : m_EditorPanels) {
			panel->init();
		}
	}

	void EditorLayer::onDetach() {
		serializeState(m_EditorState);
	}

	void EditorLayer::onEvent(std::shared_ptr<IEvent> event) {
		if (m_EditorState->temp.isInRuntimeMode && event->IsInputEvent()) {
			event->Consume();
			return; // don't propagate further
		}

		for (auto& panel : m_EditorPanels) {
			panel->onEvent(event);
			if (event->IsConsumed()) {
				break;
			}
		}
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
		for (auto& panel : m_EditorPanels) {
			panel->OnImGuiRender();
		}

		bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);

		m_ImGuiContext->EndFrame();
	}
}
