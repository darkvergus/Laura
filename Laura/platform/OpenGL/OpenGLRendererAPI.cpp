#include "platform/OpenGL/OpenGLRendererAPI.h"
#include <GL/glew.h>

#include "renderer/IComputeShader.h"

namespace Laura {

	void OpenGLRendererAPI::Init()
	{
	}

	void OpenGLRendererAPI::Clear(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetViewportSize(uint32_t width, uint32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void OpenGLRendererAPI::Dispatch(std::shared_ptr<IComputeShader> computeShader)
	{
		computeShader->Bind();
		computeShader->Dispatch();
	}

}