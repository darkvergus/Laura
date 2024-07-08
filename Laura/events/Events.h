#ifndef EVENTS_H
#define EVENTS_H

#include "lrpch.h"

#include <GLFW/glfw3.h>

#include "events/KeyCodes.h"

/**
* @brief EventType is an enum struct that represents the type of an event.
 */
enum struct EventType
{
	KEY_PRESS_EVENT,
	KEY_RELEASE_EVENT,
	KEY_REPEAT_EVENT,
	MOUSE_MOVE_EVENT,
	MOUSE_BUTTON_PRESS_EVENT,
	MOUSE_BUTTON_RELEASE_EVENT,
	MOUSE_SCROLL_EVENT
};

struct Event
{
	virtual ~Event() = default;
	EventType type;
};

struct KeyEvent : public Event
{
	virtual ~KeyEvent() = default;
	KeyCode key;
	bool keyCtrl;
	bool keyShift;
	bool keyAlt;
	bool keySuper;
	friend std::ostream& operator<<(std::ostream& COUT, const KeyEvent& EVENT);
};

struct MouseMoveEvent : public Event
{
	virtual ~MouseMoveEvent() = default;
	double xpos;
	double ypos;
	friend std::ostream& operator<<(std::ostream& COUT, const MouseMoveEvent& EVENT);
};

struct MouseButtonEvent : public Event
{
	virtual ~MouseButtonEvent() = default;
	int button;
	friend std::ostream& operator<<(std::ostream& COUT, const MouseButtonEvent& EVENT);
};

struct MouseScrollEvent : public Event
{
	virtual ~MouseScrollEvent() = default;
	double xoffset;
	double yoffset;
	friend std::ostream& operator<<(std::ostream& COUT, const MouseScrollEvent& EVENT);
};

void logEvent(Event* event);

#endif // EVENTS_H