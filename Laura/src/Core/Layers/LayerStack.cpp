#include "Core/Layers/LayerStack.h"

namespace Laura {

	LayerStack::~LayerStack()
	{
		for (std::shared_ptr<ILayer> layer : m_Layers)
		{
			layer->onDetach();
		}

		for (std::shared_ptr<ILayer> overlay : m_Overlays)
		{
			overlay->onDetach();
		}
	}


	void LayerStack::PushLayer(std::shared_ptr<ILayer> layer)
	{
		m_Layers.push_back(layer);
		layer->onAttach();
	}


	void LayerStack::PushOverlay(std::shared_ptr<ILayer> overlay)
	{
		m_Overlays.push_back(overlay);
		overlay->onAttach();
	}


	void LayerStack::PopLayer(std::shared_ptr<ILayer> layer)
	{
		m_Layers.erase(std::remove(m_Layers.begin(), m_Layers.end(), layer), m_Layers.end());
		layer->onDetach();
	}


	void LayerStack::PopOverlay(std::shared_ptr<ILayer> overlay)
	{
		m_Overlays.erase(std::remove(m_Overlays.begin(), m_Overlays.end(), overlay), m_Overlays.end());
		overlay->onDetach();
	}

	void LayerStack::onUpdate()
	{
		for (std::shared_ptr<ILayer> layer : m_Layers)
		{
			layer->onUpdate();
		}

		for (std::shared_ptr<ILayer> overlay : m_Overlays)
		{
			overlay->onUpdate();
		}
	}

	void LayerStack::onEvent(Event* event)
	{
		for (std::shared_ptr<ILayer> layer : m_Layers)
		{
			layer->onEvent(event);
		}

		for (std::shared_ptr<ILayer> overlay : m_Overlays)
		{
			overlay->onEvent(event);
		}
	}

	void LayerStack::onImGuiRender()
	{
		for (std::shared_ptr<ILayer> layer : m_Layers)
		{
			layer->onImGuiRender();
		}

		for (std::shared_ptr<ILayer> overlay : m_Overlays)
		{
			overlay->onImGuiRender();
		}
	}

}