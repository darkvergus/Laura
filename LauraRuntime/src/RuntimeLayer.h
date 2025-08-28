#pragma once

#include <Laura.h>
#include <Export/ExportSettings.h>

namespace Laura
{

	class RuntimeLayer : public ILayer {
	public:
		RuntimeLayer(std::shared_ptr<IWindow> window,
					std::shared_ptr<Profiler> profiler,
					std::shared_ptr<IEventDispatcher> eventDispatcher,
					std::shared_ptr<ProjectManager> projectManager
		);

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onEvent(std::shared_ptr<IEvent> event) override;

	private:
		void CalculateViewportCoordinates();
		// Engine Systems
		std::shared_ptr<IWindow> m_Window;
		std::shared_ptr<Profiler> m_Profiler;
		std::shared_ptr<IEventDispatcher> m_EventDispatcher; // layerstack  
		std::shared_ptr<ProjectManager> m_ProjectManager;
		
		std::shared_ptr<IImage2D> m_CurrentFrame;
		unsigned int m_Framebuffer;

		ExportSettings m_ExportSettings;

		// Viewport scaling variables
		glm::ivec4 m_ViewportCoords; // x, y, width, height for glBlitFramebuffer
		glm::ivec2 m_WindowSize;
		bool m_UpdateViewportCoordinates;
	};
}