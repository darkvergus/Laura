#pragma once

#include "Laura.h"
#include "EditorState.h"
#include "EditorUI/IEditorPanel.h"

namespace Laura
{

	class MainMenuPanel : public IEditorPanel {
	public:
		MainMenuPanel(std::shared_ptr<EditorState> editorState, 
					  std::shared_ptr<IEventDispatcher> eventDispatcher, 
			          std::shared_ptr<ProjectManager> projectManager)
			: m_EditorState(editorState)
			, m_EventDispatcher(eventDispatcher)
			, m_ProjectManager(projectManager) 
		{}

		~MainMenuPanel() = default;

		virtual inline void init() override {}
		virtual void OnImGuiRender() override;
		virtual inline void onEvent(std::shared_ptr<IEvent> event) override {}

	private:
		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<IEventDispatcher> m_EventDispatcher;
		std::shared_ptr<ProjectManager> m_ProjectManager;
	};
}