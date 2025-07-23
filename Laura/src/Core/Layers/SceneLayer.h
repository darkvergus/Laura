#pragma once
#include "lrpch.h"

#include "Core/Layers/ILayer.h"
#include "Core/Layers/LayerStack.h"
#include "Core/Events/IEvent.h"

#include "Scene/Scene.h"

namespace Laura
{

	class SceneLayer : public ILayer {
	public:
		SceneLayer(std::shared_ptr<IEventDispatcher> eventDispatcher);

		inline std::shared_ptr<Scene> GetScene() const { return m_Scene; }

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onImGuiRender() override;
		virtual void onEvent(std::shared_ptr<IEvent> event) override;
	private:
		std::shared_ptr<IEventDispatcher> m_EventDispatcher;
		std::shared_ptr<Scene> m_Scene;
	};
}
