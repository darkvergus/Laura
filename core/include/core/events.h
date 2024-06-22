#ifndef EVENTS_H
#define EVENTS_H

#include <GLFW/glfw3.h>
#include <set>
#include <vector>

/**
* @brief EventType is an enum struct that represents the type of an event.
 */
enum struct EventType
{
	KEY_PRESS_EVENT,
	KEY_RELEASE_EVENT,
	MOUSE_MOVE_EVENT,
	MOUSE_BUTTON_PRESS_EVENT,
	MOUSE_BUTTON_RELEASE_EVENT,
	MOUSE_SCROLL_EVENT
};


/**
* @brief Event is a struct that represents an input event. It contains the type of the event
 * and additional data depending on the type of the event.
 */
struct Event {
	EventType type;
	union {
		struct {
			int key;
			int scancode;
			int action;
			int mods;
		} keyEvent;
		struct {
			double xpos;
			double ypos;
		} mouseMoveEvent;
		struct {
			int button;
			int action;
			int mods;
		} mouseButtonEvent;
		struct {
			double xoffset;
			double yoffset;
		} scrollEvent;
	};
};


/**
* @brief EventListener is an interface for classes that want to listen to events.
 * The idea is that the EventDispatcher will call the onEvent method of all the listeners
 * when an event occurs. This way, the listeners can implement their own logic for handling
 * input events.
 */
class EventListener
{
public:
	virtual void onEvent(Event event) = 0;
};

/**
* @brief EventDispatcher is a class that listens to GLFW events and dispatches them to
 * all the listeners that are registered to it.
 */
class EventDispatcher
{
public:
	EventDispatcher(GLFWwindow* window);

	void addListener(EventListener* listener);
	void removeListener(EventListener* listener);

private:
	GLFWwindow* m_Window;
	std::set <EventListener*> m_Listeners;

	/// These are callback methods called by GLFW when an event occurs.
	/// They are static methods because GLFW requires them to be static. The method gets the EventDispatcher
	/// object of the current instance (self) from the GLFW window user pointer to perform actions as if the function was not static.
	static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void GLFWMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	void notifyListeners(Event event);
};

#endif // EVENTS_H