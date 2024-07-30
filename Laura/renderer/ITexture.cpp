#include "Renderer/ITexture.h"
#include "Renderer/IRendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Laura {

	std::shared_ptr<ITexture> ITexture::Create(const std::string& filepath, int bindingPoint)
	{
		switch (IRendererAPI::GetAPI())
		{
		case IRendererAPI::API::None: LR_CORE_CRITICAL("in ITexture::Create() - RendererAPI::None UNSUPPORTED"); return nullptr;
		case IRendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture>(filepath, bindingPoint);
		}
	}

	std::shared_ptr<ITexture> ITexture::Create(int width, int height, int channels, int bindingPoint)
	{
		switch (IRendererAPI::GetAPI())
		{
		case IRendererAPI::API::None: LR_CORE_CRITICAL("in ITexture::Create() - RendererAPI::None UNSUPPORTED"); return nullptr;
		case IRendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture>(width, height, channels, bindingPoint);
		}
	}

}