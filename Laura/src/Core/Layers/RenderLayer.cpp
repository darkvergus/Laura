#include "Core/Layers/RenderLayer.h"
#include "Core/Events/RenderEvents.h"

namespace Laura
{

	RenderLayer::RenderLayer(std::shared_ptr<IEventDispatcher> eventDispatcher, 
							 std::shared_ptr<Profiler> profiler,
							 std::shared_ptr<const ProjectManager> projectManager)
		:	m_EventDispatcher(eventDispatcher), 
			m_Profiler(profiler), 
			m_ProjectManager(projectManager),
			m_Renderer(profiler) {
	}

	void RenderLayer::onAttach() {
		m_Renderer.settings.raysPerPixel = 1;
		m_Renderer.settings.bouncesPerRay = 5;
		m_Renderer.settings.maxAABBIntersections = 500;
		m_Renderer.settings.displayBVH = false;
		m_Renderer.settings.ShouldAccumulate = true;

		// The FRAME_WIDTH and FRAME_HEIGHT define the dimensions of the render frame.
		// These values represent the actual number of pixels that the renderer will process to produce the final image.
		m_Renderer.settings.Resolution = glm::uvec2(1200, 800);
		m_Renderer.settings.ComputeShaderPath = LR_RESOURCES_PATH "Shaders/PathTracing.comp";
		m_Renderer.Init();
	}

	void RenderLayer::onDetach() {
	}

	void RenderLayer::onUpdate() {
		if (m_ProjectManager->ProjectIsOpen()) { // Get...Manager should not return nullptr
			const auto& scene = m_ProjectManager->GetSceneManager()->GetOpenScene();
			const auto& assetPool = m_ProjectManager->GetAssetManager()->GetAssetPool();
			std::shared_ptr<IImage2D> RenderedFrame = m_Renderer.Render(scene.get(), assetPool.get());
			m_EventDispatcher->dispatchEvent(std::make_shared<NewFrameRenderedEvent>(RenderedFrame));
		}
	}

	void RenderLayer::onEvent(std::shared_ptr<IEvent> event) {
	}
}
