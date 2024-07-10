#include "renderer/IComputeShader.h"
#include "renderer/Renderer.h"

#include "platform/OpenGL/shaders/OpenGLComputeShader.h"


std::shared_ptr<IComputeShader> IComputeShader::Create(const std::string& filepath, const glm::uvec3& workGroupSizes)
{
	switch (Renderer::GetAPI())
	{
	case IRendererAPI::API::None: LR_CORE_CRITICAL("RendererAPI::None - UNSUPPORTED"); return nullptr;
	case IRendererAPI::API::OpenGL : return std::make_shared<OpenGLComputeShader>(filepath, workGroupSizes);
	}
}
