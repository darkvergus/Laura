#pragma once
#include "lrpch.h"

#include "Core/Layers/ILayer.h"
#include "Core/Layers/LayerStack.h"
#include "Core/Events/IEvent.h"
#include "Project/Scene/Scene.h"
#include "Project/Assets/AssetManager.h"

namespace Laura
{

	class SceneLayer : public ILayer {
	public:
		SceneLayer(std::shared_ptr<IEventDispatcher> eventDispatcher, std::shared_ptr<AssetManager> assetManager);

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onEvent(std::shared_ptr<IEvent> event) override;
	private:
		std::shared_ptr<IEventDispatcher> m_EventDispatcher;
		std::shared_ptr<AssetManager> m_AssetManager;
		std::shared_ptr<Scene> m_Scene;
	};
}
