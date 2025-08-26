#pragma once

#include <glm/glm.hpp>
#include "Core/Events/IEvent.h"

namespace Laura 
{

	struct WindowResizeEvent: public IEvent {
		glm::vec2 windowDims;

		WindowResizeEvent(glm::vec2 windowDims) 
			: windowDims(std::move(windowDims)) {}

		inline EventType GetType() const override { return EventType::WINDOW_RESIZE_EVENT; }
	};
}