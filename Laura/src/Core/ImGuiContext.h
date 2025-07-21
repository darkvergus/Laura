#pragma once

#include "Core/Layers/ILayer.h"
#include "Core/IWindow.h"

namespace Laura {

	class ImGuiContext
	{
	public:
		ImGuiContext(std::shared_ptr<IWindow> window);
		virtual ~ImGuiContext();

		void Init();
		void BeginFrame();
		void EndFrame();

	private:
		std::shared_ptr<IWindow> m_Window;
	};

}