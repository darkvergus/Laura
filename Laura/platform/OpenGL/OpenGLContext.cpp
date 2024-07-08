#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "platform/OpenGL/OpenGLContext.h"
#include "core/Log.h"

void OpenGLContext::setWindowHints()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

OpenGLContext::OpenGLContext(GLFWwindow* window)
	: m_NativeWindow(window)
{
}

void OpenGLContext::init()
{
	glfwMakeContextCurrent(m_NativeWindow);
	
	if (glewInit() != GLEW_OK)
	{
		CORE_CRITICAL("Failed to initialize GLEW!");
	}

	CORE_INFO("Using OpenGL - Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	CORE_INFO("Successfully initialized OpenGL context and GLEW!");
}

void OpenGLContext::swapBuffers()
{
	glfwSwapBuffers(m_NativeWindow);
}