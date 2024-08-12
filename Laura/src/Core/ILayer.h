#pragma once

#include "events/events.h"
#include "core/IWindow.h"

namespace Laura {

	class ILayer
	{

	public:
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

}