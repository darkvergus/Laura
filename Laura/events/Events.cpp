#include <GLFW/glfw3.h>
#include "events/events.h"

namespace Laura {

	void logEvent(Event* event)
	{
		switch (event->type)
		{
		case EventType::KEY_PRESS_EVENT:
		case EventType::KEY_RELEASE_EVENT:
		case EventType::KEY_REPEAT_EVENT:
			std::cout << *dynamic_cast<KeyEvent*>(event) << std::endl;
			return;
		case EventType::MOUSE_MOVE_EVENT:
			std::cout << *dynamic_cast<MouseMoveEvent*>(event) << std::endl;
			return;
		case EventType::MOUSE_BUTTON_PRESS_EVENT:
		case EventType::MOUSE_BUTTON_RELEASE_EVENT:
			std::cout << *dynamic_cast<MouseButtonEvent*>(event) << std::endl;
			return;
		case EventType::MOUSE_SCROLL_EVENT:
			std::cout << *dynamic_cast<MouseScrollEvent*>(event) << std::endl;
			return;
		}
	}

	std::ostream& operator<<(std::ostream& COUT, const KeyEvent& EVENT)
	{
		switch (EVENT.type)
		{
		case EventType::KEY_PRESS_EVENT:
			COUT << "KEY_PRESS_EVENT | ";
			break;
		case EventType::KEY_RELEASE_EVENT:
			COUT << "KEY_RELEASE_EVENT | ";
			break;
		case EventType::KEY_REPEAT_EVENT:
			COUT << "KEY_REPEAT_EVENT | ";
			break;
		}
		COUT << "Key: " << EVENT.key << " Alt: " << EVENT.keyAlt << " Ctrl: " << EVENT.keyCtrl << " Shift: " << EVENT.keyShift << " Super: " << EVENT.keySuper << "\n";
		return COUT;
	}

	std::ostream& operator<<(std::ostream& COUT, const MouseMoveEvent& EVENT)
	{
		COUT << "MOUSE_MOVE_EVENT | " << "Mouse x: " << EVENT.xpos << " Mouse y: " << EVENT.ypos << "\n";
		return COUT;
	}

	std::ostream& operator<<(std::ostream& COUT, const MouseButtonEvent& EVENT)
	{
		std::string StrType = (EVENT.type == EventType::MOUSE_BUTTON_PRESS_EVENT) ? "MOUSE_BUTTON_PRESS" : "MOUSE_BUTTON_RELEASE";
		COUT << StrType << " | " << EVENT.button << "\n";
		return COUT;
	}

	std::ostream& operator<<(std::ostream& COUT, const MouseScrollEvent& EVENT)
	{
		COUT << "MOUSE_SCROLL_EVENT | " << "X offset: " << EVENT.xoffset << " Y offset: " << EVENT.yoffset << "\n";
		return COUT;
	}

}