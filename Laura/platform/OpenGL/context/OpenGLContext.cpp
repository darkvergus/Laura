#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "platform/OpenGL/context/OpenGLContext.h"
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
		LR_CORE_CRITICAL("[ERROR] Failed to initialize GLEW!");
	}

	LR_CORE_INFO("Using OpenGL - Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	LR_CORE_INFO("Successfully initialized OpenGL context and GLEW!");
}

void OpenGLContext::swapBuffers()
{
	glfwSwapBuffers(m_NativeWindow);
}