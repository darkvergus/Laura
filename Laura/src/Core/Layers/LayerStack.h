#pragma once

#include "lrpch.h"

#include "ILayer.h"

namespace Laura {

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(std::shared_ptr<ILayer> layer);
		void PushOverlay(std::shared_ptr<ILayer> overlay);
		void PopLayer(std::shared_ptr<ILayer> layer);
		void PopOverlay(std::shared_ptr<ILayer> overlay);

		void onUpdate();
		void onEvent(Event* event);
		void onImGuiRender();

		std::vector<std::shared_ptr<ILayer>> m_Layers;
		std::vector<std::shared_ptr<ILayer>> m_Overlays;
	};

}