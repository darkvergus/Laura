#include "lrpch.h"

#include "GLFWWindow.h"
#include "Core/Events/KeyEvents.h"
#include "Core/Events/MouseEvents.h"
#include "core/Log.h"

namespace Laura {

	GLFWWindowIMPL::GLFWWindowIMPL(const WindowProps& windowProps)
		: m_WindowProps(windowProps) {

		if (!glfwInit()){
			LOG_ENGINE_CRITICAL("Failed to initialize GLFW!");
		}

		OpenGLContext::setWindowHints();

		m_NativeWindow = glfwCreateWindow(m_WindowProps.width, m_WindowProps.height, (m_WindowProps.title).c_str(), NULL, NULL);
		if (!m_NativeWindow) {
			LOG_ENGINE_CRITICAL("Failed to generate GLFW window!");
		}

		m_Context = new OpenGLContext(m_NativeWindow);
		m_Context->init();

		glfwSwapInterval(m_WindowProps.VSync);

		glfwSetWindowUserPointer(m_NativeWindow, this);
		glfwSetKeyCallback(m_NativeWindow, GLFWKeyCallback);
		glfwSetMouseButtonCallback(m_NativeWindow, GLFWMouseButtonCallback);
		glfwSetCursorPosCallback(m_NativeWindow, GLFWMousePositionCallback);
		glfwSetScrollCallback(m_NativeWindow, GLFWScrollCallback);
	}

	GLFWWindowIMPL::~GLFWWindowIMPL() {
		glfwTerminate();
	}

	int GLFWWindowIMPL::getWidth() const {
		return m_WindowProps.width;
	}

	int GLFWWindowIMPL::getHeight() const {
		return m_WindowProps.height;
	}

	bool GLFWWindowIMPL::isVSync() const {
		return m_WindowProps.VSync;
	}

	void GLFWWindowIMPL::setVSync(bool enabled) {
		m_WindowProps.VSync = enabled;
		glfwSwapInterval(m_WindowProps.VSync);
	}

	void GLFWWindowIMPL::onUpdate() {
		glfwPollEvents();
		m_Context->swapBuffers();
	}

	void* GLFWWindowIMPL::getNativeWindow() const {
		return m_NativeWindow;
	}

	// takes a const reference to a function returning void and argument std::shared_ptr<IEvent>
	void GLFWWindowIMPL::setEventCallback(const std::function<void(std::shared_ptr<IEvent>)>& callback) {
		dispatchEvent = callback;
	}

#define thisWindow static_cast<GLFWWindowIMPL*>(glfwGetWindowUserPointer(window))

	void GLFWWindowIMPL::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		KeyCode k = static_cast<KeyCode>(key);
		bool ctrl = (mods & GLFW_MOD_CONTROL) != 0;
		bool shift = (mods & GLFW_MOD_SHIFT) != 0;
		bool alt = (mods & GLFW_MOD_ALT) != 0;
		bool super = (mods & GLFW_MOD_SUPER) != 0;

		switch (action) {
			case GLFW_PRESS:
				thisWindow->dispatchEvent(std::make_shared<KeyPressEvent>(k, ctrl, shift, alt, super));
				return;
			case GLFW_RELEASE:
				thisWindow->dispatchEvent(std::make_shared<KeyReleaseEvent>(k, ctrl, shift, alt, super));
				return;
			case GLFW_REPEAT:
				thisWindow->dispatchEvent(std::make_shared<KeyRepeatEvent>(k, ctrl, shift, alt, super));
				return;
		}
	}

	void GLFWWindowIMPL::GLFWMousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
		thisWindow->dispatchEvent(std::make_shared<MouseMoveEvent>(xpos, ypos));
	}

	void GLFWWindowIMPL::GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		MouseCode b = static_cast<MouseCode>(button);

		switch (action) {
			case GLFW_PRESS:
				thisWindow->dispatchEvent(std::make_shared<MouseButtonPressEvent>(button));
				return;
			case GLFW_RELEASE:
				thisWindow->dispatchEvent(std::make_shared<MouseButtonReleaseEvent>(button));
				return;
		}
	}

	void GLFWWindowIMPL::GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		thisWindow->dispatchEvent(std::make_shared<MouseScrollEvent>(xoffset, yoffset));
	}

	bool GLFWWindowIMPL::isKeyPressed(KeyCode key) {
		auto state = glfwGetKey(m_NativeWindow, key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool GLFWWindowIMPL::isMouseButtonPressed(MouseCode button) {
		auto state = glfwGetMouseButton(m_NativeWindow, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> GLFWWindowIMPL::getMousePosition() {
		double xpos, ypos;
		glfwGetCursorPos(m_NativeWindow, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	bool GLFWWindowIMPL::shouldClose() {
		return glfwWindowShouldClose(m_NativeWindow);
	}
}