#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "ILayer.h"
#include <vector>

class LayerStack
{
public:
	LayerStack() = default;
	~LayerStack();
	
	void PushLayer(ILayer* layer);
	void PushOverlay(ILayer* overlay);
	void PopLayer(ILayer* layer);
	void PopOverlay(ILayer* overlay);

	void onUpdate();
	void dispatchEvent(Event* event);

	std::vector<ILayer*> m_Layers;
	std::vector<ILayer*> m_Overlays;
};

#endif // LAYERSTACK_H