#ifndef ILAYER_H
#define ILAYER_H

#include "events/events.h"
#include "core/IWindow.h"

class ILayer
{

public:
	ILayer(IWindow* window);
	virtual ~ILayer() = default;

	virtual void onAttach() {};
	virtual void onDetach() {};
	virtual void onUpdate() {};
	virtual void onImGuiRender() {};
	virtual void onEvent(Event* event) {};

	inline void setLayerName(const std::string& name) { m_LayerName = name; }
	inline std::string getLayerName() const { return m_LayerName; }

protected:
	IWindow* m_Window;
	std::string m_LayerName;
};

#endif // ILAYER_H