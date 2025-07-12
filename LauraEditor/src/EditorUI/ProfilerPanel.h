#pragma once

#include "Laura.h"
#include "EditorState.h"

namespace Laura
{

	class ProfilerPanel
	{
	public:
		ProfilerPanel(std::shared_ptr<EditorState> editorState)
			: m_EditorState(editorState) {}

		~ProfilerPanel() = default;

		void OnImGuiRender(std::shared_ptr<Profiler> profiler);
	private:
		
		struct plotLineStyle {
			ImVec4 color;
			int colormapID;
			float thickness;
		};

		std::shared_ptr<EditorState> m_EditorState;
	};
}