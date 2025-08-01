#pragma once

#include "lrpch.h"
#include "Project/Scene/Scene.h"
#include "Core/Layers/ILayer.h"
#include "Core/Layers/LayerStack.h"
#include "Core/Events/IEvent.h"
#include "Renderer/Renderer.h"
#include "Project/Assets/AssetManager.h"

namespace Laura
{

	class RenderLayer : public ILayer {
	public:
		RenderLayer(std::shared_ptr<IEventDispatcher> eventDispatcher,
					std::shared_ptr<Profiler> profiler, // will go in the future
					std::shared_ptr<AssetPool> resourcePool
		);

		virtual void onAttach() override;
		virtual void onDetach() override;

		// onUpdate RenderLayer dispatches an event with the rendered texture
		virtual void onUpdate() override;
		virtual void onEvent(std::shared_ptr<IEvent> event) override;

	private:
		std::shared_ptr<IEventDispatcher> m_EventDispatcher;
		std::shared_ptr<Profiler> m_Profiler;
		std::shared_ptr<Renderer> m_Renderer;

		std::shared_ptr<const AssetPool> m_AssetPool;

		std::weak_ptr<const Scene> m_Scene;
		std::shared_ptr<IImage2D> m_RenderBuffer;
	};
}
