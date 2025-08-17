#pragma once

#include "Laura.h"
#include "EditorState.h"
#include "EditorUI/IEditorPanel.h"

namespace Laura
{

	class ExportPanel : public IEditorPanel {
	public:
		ExportPanel(std::shared_ptr<EditorState> editorState, 
			        std::shared_ptr<ProjectManager> projectManager)
			: m_EditorState(editorState)
			, m_ProjectManager(projectManager) 
		{}

		~ExportPanel() = default;

		virtual inline void init() override {}
		virtual void OnImGuiRender() override;
		virtual inline void onEvent(std::shared_ptr<IEvent> event) override {}

	private:
		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<IEventDispatcher> m_EventDispatcher;
		std::shared_ptr<ProjectManager> m_ProjectManager;
	};
}