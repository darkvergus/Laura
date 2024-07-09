#ifndef GLFW_WINDOW_IMPL_H
#define GLFW_WINDOW_IMPL_H

#include <GLFW/glfw3.h>

#include "core/IWindow.h"
#include "platform/OpenGL/context/OpenGLContext.h"


class GLFWWindowIMPL : public IWindow
{
public:
	GLFWWindowIMPL(const WindowProps& windowProps);
	~GLFWWindowIMPL();

	void onUpdate() override;

	int getWidth() const override;
	int getHeight() const override;
	bool isVSync() const override;

	void setVSync(bool enabled) override;
	void* getNativeWindow() const override;
	

	///  input polling
	bool isKeyPressed(KeyCode key) override;
	bool isMouseButtonPressed(MouseCode) override;
	std::pair<float, float> getMousePosition() override;
	bool shouldClose() override;
	

	void setEventCallback(const std::function<void(Event*)>& callback) override;



private:
	GLFWwindow* m_NativeWindow;
	OpenGLContext* m_Context;
	WindowProps m_WindowProps;
	std::function<void(Event*)> notifyDispatcher;

	/// These are callback methods called by GLFW when an event occurs.
	/// They are static methods because GLFW requires them to be static. The method gets the EventDispatcher
	/// object of the current instance (self) from the GLFW window user pointer to perform actions as if the function was not static.
	static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void GLFWMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif