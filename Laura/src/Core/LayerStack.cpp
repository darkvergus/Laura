#include "LayerStack.h"
#include "core/LayerStack.h"

namespace Laura {

	LayerStack::~LayerStack()
	{
		for (ILayer* layer : m_Layers)
		{
			delete layer;
		}

		for (ILayer* overlay : m_Overlays)
		{
			delete overlay;
		}
	}


	void LayerStack::PushLayer(ILayer* layer)
	{
		m_Layers.push_back(layer);
		layer->onAttach();
	}


	void LayerStack::PushOverlay(ILayer* overlay)
	{
		m_Overlays.push_back(overlay);
		overlay->onAttach();
	}


	void LayerStack::PopLayer(ILayer* layer)
	{
		m_Layers.erase(std::remove(m_Layers.begin(), m_Layers.end(), layer), m_Layers.end());
		layer->onDetach();
	}


	void LayerStack::PopOverlay(ILayer* overlay)
	{
		m_Overlays.erase(std::remove(m_Overlays.begin(), m_Overlays.end(), overlay), m_Overlays.end());
		overlay->onDetach();
	}

	void LayerStack::onUpdate()
	{
		for (ILayer* layer : m_Layers)
		{
			layer->onUpdate();
		}

		for (ILayer* overlay : m_Overlays)
		{
			overlay->onUpdate();
		}
	}

	void LayerStack::onEvent(Event* event)
	{
		for (ILayer* layer : m_Layers)
		{
			layer->onEvent(event);
		}

		for (ILayer* overlay : m_Overlays)
		{
			overlay->onEvent(event);
		}
	}

	void LayerStack::onImGuiRender()
	{
		for (ILayer* layer : m_Layers)
		{
			layer->onImGuiRender();
		}

		for (ILayer* overlay : m_Overlays)
		{
			overlay->onImGuiRender();
		}
	}

}