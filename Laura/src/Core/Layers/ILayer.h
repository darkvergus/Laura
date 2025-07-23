#pragma once

#include "Core/Events/IEvent.h"
#include "Core/IWindow.h"

namespace Laura 
{
	class ILayer {
	public:
		virtual ~ILayer() = default;

		virtual void onAttach() {};
		virtual void onDetach() {};
		virtual void onUpdate() {};
		virtual void onImGuiRender() {};
		virtual void onEvent(std::shared_ptr<IEvent> event) {};
	};

}