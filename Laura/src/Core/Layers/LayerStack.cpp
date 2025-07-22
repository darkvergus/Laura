#include "Core/Layers/LayerStack.h"

namespace Laura 
{
	LayerStack::~LayerStack() {
		for (std::shared_ptr<ILayer> layer : m_Layers) {
			layer->onDetach();
		}
	}

	void LayerStack::PushLayer(std::shared_ptr<ILayer> layer) {
		m_Layers.push_back(layer);
		layer->onAttach();
	}

	void LayerStack::PopLayer(std::shared_ptr<ILayer> layer) {
		m_Layers.erase(std::remove(m_Layers.begin(), m_Layers.end(), layer), m_Layers.end());
		layer->onDetach();
	}

	void LayerStack::onUpdate() {
		for (std::shared_ptr<ILayer> layer : m_Layers) {
			layer->onUpdate();
		}
	}

	void LayerStack::dispatchEvent(Event* event) {
		for (std::shared_ptr<ILayer> layer : m_Layers) {
			layer->onEvent(event);
			/* consuming of events in the future
			if (event.consumed) {
				break;
			}
			*/
		}
	}

	void LayerStack::onImGuiRender() {
		for (std::shared_ptr<ILayer> layer : m_Layers) {
			layer->onImGuiRender();
		}
	}
}