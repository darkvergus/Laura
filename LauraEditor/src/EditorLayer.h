#pragma once

#include <Laura.h>
#include "EditorState.h"
#include "ImGuiContext.h"
#include "EditorUI/Launcher/Launcher.h"
#include "EditorUI/ViewportPanel/ViewportPanel.h"
#include "EditorUI/SceneHierarchyPanel/SceneHierarchyPanel.h"
#include "EditorUI/InspectorPanel/InspectorPanel.h"
#include "EditorUI/ProfilerPanel/ProfilerPanel.h"
#include "EditorUI/RenderSettingsPanel/RenderSettingsPanel.h"
#include "EditorUI/ThemePanel/ThemePanel.h"
#include "EditorUI/AssetsPanel/AssetsPanel.h"

namespace Laura
{

	class EditorLayer : public ILayer {
	public:
		EditorLayer(std::shared_ptr<Profiler> profiler,
					std::weak_ptr<IEventDispatcher> eventDispatcher,
					std::shared_ptr<ProjectManager> projectManager,
					std::shared_ptr<ImGuiContext> imGuiContext
		);

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onEvent(std::shared_ptr<IEvent> event) override;

	private:
		void DrawMainMenu();

		// Engine
		std::shared_ptr<Profiler> m_Profiler;
		std::shared_ptr<IEventDispatcher> m_EventDispatcher;
		std::shared_ptr<ProjectManager> m_ProjectManager;

		// Values received from events
		std::weak_ptr<Scene> m_Scene;
		std::weak_ptr<IImage2D> m_LatestFrameRender;

		// Editor
		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<ImGuiContext> m_ImGuiContext;

		Launcher m_Launcher;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		InspectorPanel m_InspectorPanel;
		ViewportPanel m_ViewportPanel;
		ThemePanel m_ThemePanel;
		ProfilerPanel m_ProfilerPanel;
		RenderSettingsPanel m_RenderSettingsPanel;
		AssetsPanel m_AssetsPanel;
	};
}