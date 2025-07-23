#pragma once 

#include "Laura.h"
#include "EditorState.h"

namespace Laura
{

	class ThemePanel {
	public:
		inline ThemePanel(std::shared_ptr<EditorState> editorState)
			: m_EditorState(editorState) {
		}

		~ThemePanel() = default;
		void OnImGuiRender();

	private:
		std::shared_ptr<EditorState> m_EditorState;
	};
}