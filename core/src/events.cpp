#include "core/events.h"
#include <iostream>

EventDispatcher::EventDispatcher(GLFWwindow* window) : m_Window(window)
{
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetKeyCallback(m_Window, GLFWKeyCallback);
    glfwSetMouseButtonCallback(m_Window, GLFWMouseButtonCallback);
    glfwSetCursorPosCallback(m_Window, GLFWMousePositionCallback);
    glfwSetScrollCallback(m_Window, GLFWScrollCallback);
}

#define thisDispatcher static_cast<EventDispatcher*>(glfwGetWindowUserPointer(window))

void EventDispatcher::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    EventDispatcher* eventDispatcher = thisDispatcher;
    Event event;
    event.type = (action == GLFW_PRESS) ? EventType::KEY_PRESS_EVENT : EventType::KEY_RELEASE_EVENT;
    event.keyEvent.key = key;
    event.keyEvent.scancode = scancode;
    event.keyEvent.action = action;
    event.keyEvent.mods = mods;
    thisDispatcher->notifyListeners(event);
}


void EventDispatcher::GLFWMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    EventDispatcher* eventDispatcher = thisDispatcher;
    Event event;
    event.type = EventType::MOUSE_MOVE_EVENT;
    event.mouseMoveEvent.xpos = xpos;
    event.mouseMoveEvent.ypos = ypos;
    thisDispatcher->notifyListeners(event);
}


void EventDispatcher::GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    EventDispatcher* eventDispatcher = thisDispatcher;
    Event event;
    event.type = (action == GLFW_PRESS) ? EventType::MOUSE_BUTTON_PRESS_EVENT : EventType::MOUSE_BUTTON_RELEASE_EVENT;
    event.mouseButtonEvent.button = button;
    event.mouseButtonEvent.action = action;
    event.mouseButtonEvent.mods = mods;
    thisDispatcher->notifyListeners(event);
}


void EventDispatcher::GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    EventDispatcher* eventDispatcher = thisDispatcher;
    Event event;
    event.type = EventType::MOUSE_SCROLL_EVENT;
    event.scrollEvent.xoffset = xoffset;
    event.scrollEvent.yoffset = yoffset;
    thisDispatcher->notifyListeners(event);
}


void EventDispatcher::addListener(EventListener* listener)
{
    m_Listeners.insert(listener);
}


void EventDispatcher::removeListener(EventListener* listener)
{
    m_Listeners.erase(listener);
}


void EventDispatcher::notifyListeners(Event event)
{
    for (EventListener* listener : m_Listeners) 
    {
		listener->onEvent(event);
	}
}




