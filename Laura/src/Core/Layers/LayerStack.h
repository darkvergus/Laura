#pragma once

#include "lrpch.h"
#include "Core/Layers/ILayer.h"

namespace Laura {
	// passed to layers to only access dispatchEvent() without access to the entire layerstack
	class IEventDispatcher { 
	public:
		virtual void dispatchEvent(Event* event) = 0;
		virtual ~IEventDispatcher() = default;
	};

	class LayerStack : public IEventDispatcher {
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(std::shared_ptr<ILayer> layer);
		void PopLayer(std::shared_ptr<ILayer> layer);

		void onUpdate();
		void onImGuiRender();
		virtual void dispatchEvent(Event* event) override;

	private:
		std::vector<std::shared_ptr<ILayer>> m_Layers;
	};
}