#include "Renderer/IComputeShader.h"
#include "Renderer/IRendererAPI.h"
#include "platform/OpenGL/OpenGLComputeShader.h"

namespace Laura {

	std::shared_ptr<IComputeShader> IComputeShader::Create(const std::string& filepath, const glm::uvec3& workGroupSizes)
	{
		switch (IRendererAPI::GetAPI())
		{
		case IRendererAPI::API::None: LR_CORE_CRITICAL("RendererAPI::None - UNSUPPORTED"); return nullptr;
		case IRendererAPI::API::OpenGL: return std::make_shared<OpenGLComputeShader>(filepath, workGroupSizes);
		}
	}

}