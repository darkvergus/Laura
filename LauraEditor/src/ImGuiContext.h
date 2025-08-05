#pragma once

#include "Laura.h"

namespace Laura 
{
	struct ImGuiContextFontRegistry {
		ImFont* HighResIcons = nullptr;
	};

	class ImGuiContext {
	public:
		ImGuiContext(std::shared_ptr<IWindow> window);
		virtual ~ImGuiContext();

		void Init();
		void BeginFrame();
		void EndFrame();

	private:
		std::shared_ptr<ImGuiContextFontRegistry> m_FontRegistry;
		std::shared_ptr<IWindow> m_Window;
	};
}