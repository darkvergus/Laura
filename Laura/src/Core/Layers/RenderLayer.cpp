#include "Core/Layers/RenderLayer.h"
#include "Core/Events/RenderEvents.h"
#include "Core/Events/SceneEvents.h"

namespace Laura
{

	RenderLayer::RenderLayer(std::shared_ptr<IEventDispatcher> eventDispatcher, 
							 std::shared_ptr<Profiler> profiler,
							 std::shared_ptr<ResourcePool> resourcePool)
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
		m_Renderer->settings.Resolution = glm::uvec2(1200, 800);
		m_Renderer->settings.ComputeShaderPath = LR_RESOURCES_PATH "Shaders/PathTracing.comp";
		m_Renderer->Init();
	}

	void RenderLayer::onDetach() {
	}

	void RenderLayer::onUpdate() {
		std::shared_ptr<IImage2D> RenderedFrame = m_Renderer->Render(m_Scene.lock().get(), m_ResourcePool.get());
		m_EventDispatcher->dispatchEvent(std::make_shared<NewFrameRenderedEvent>(RenderedFrame));
	}

	void RenderLayer::onEvent(std::shared_ptr<IEvent> event) {
		if (event->GetType() == EventType::SCENE_LOADED_EVENT) {
			m_Scene = std::dynamic_pointer_cast<SceneLoadedEvent>(event)->scene;
		}
	}
}
