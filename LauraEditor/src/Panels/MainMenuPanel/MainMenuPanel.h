#pragma once

#include "Laura.h"
#include "EditorState.h"
#include "Panels/IEditorPanel.h"
#include "ImGuiContext.h"

namespace Laura
{

	class MainMenuPanel : public IEditorPanel {
	public:
		MainMenuPanel(std::shared_ptr<EditorState> editorState, 
					  std::shared_ptr<IEventDispatcher> eventDispatcher, 
			          std::shared_ptr<ProjectManager> projectManager,
					  std::shared_ptr<ImGuiContext> imGuiContext)
			: m_EditorState(editorState)
			, m_EventDispatcher(eventDispatcher)
			, m_ProjectManager(projectManager)
			, m_ImGuiContext(imGuiContext)
		{}

		~MainMenuPanel() = default;

		virtual inline void init() override {}
		virtual void OnImGuiRender() override;
		virtual inline void onEvent(std::shared_ptr<IEvent> event) override {}

	private:
		bool m_ShouldOpenProject = false, m_ShouldCloseProject = false;
		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<IEventDispatcher> m_EventDispatcher;
		std::shared_ptr<ProjectManager> m_ProjectManager;
		std::shared_ptr<ImGuiContext> m_ImGuiContext;
	};
}