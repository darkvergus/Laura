#include "Core\Layers\RenderLayer.h"

namespace Laura
{
	RenderLayer::RenderLayer(std::weak_ptr<IEventDispatcher> eventDispatcher, 
							 std::shared_ptr<Profiler> profiler,
							 std::shared_ptr<Asset::ResourcePool> resourcePool)
		:	m_EventDispatcher(eventDispatcher), 
			m_Profiler(profiler), 
			m_ResourcePool(resourcePool),
			m_Renderer(std::make_shared<Renderer>(profiler)) {
	}

	void RenderLayer::onAttach() {
		m_Renderer->settings.raysPerPixel = 1;
		m_Renderer->settings.bouncesPerRay = 5;
		m_Renderer->settings.maxAABBIntersections = 500;
		m_Renderer->settings.displayBVH = false;
		m_Renderer->settings.ShouldAccumulate = false;

		// The FRAME_WIDTH and FRAME_HEIGHT define the dimensions of the render frame.
		// These values represent the actual number of pixels that the renderer will process to produce the final image.
		// Note: These dimensions are different from the size or aspect ratio of the ImGui viewport window in the editor.
		// The camera's aspect ratio only stretches the image to fit the viewport window correctly
		m_Renderer->settings.Resolution = glm::uvec2(1200, 800);
		m_Renderer->settings.ComputeShaderPath = LR_RESOURCES_PATH "Shaders/PathTracing.comp";
		m_Renderer->Init();
	}

	void RenderLayer::onDetach() {
	}

	void RenderLayer::onUpdate() {
		if (m_Scene && m_ResourcePool) {
			std::shared_ptr<IImage2D> RenderedFrame = m_Renderer->Render(m_Scene.get(), m_ResourcePool.get());
			//m_EventDispatcher->DispatchEvent(FrameRenderEvent{renderedFrame});
		}
	}

	void RenderLayer::onImGuiRender() {
		// no rendering in this layer
	}

	void RenderLayer::onEvent(Event* event) {
		// no events should get here 
	}
}
