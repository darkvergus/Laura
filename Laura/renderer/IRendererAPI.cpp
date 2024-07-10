#include "renderer/IRendererAPI.h"

#include "platform/opengl/OpenGLRendererAPI.h"

////////////////////////////
/// SET THE RENDERER API ///
////////////////////////////
IRendererAPI::API IRendererAPI::s_API = IRendererAPI::API::OpenGL;

std::shared_ptr<IRendererAPI> IRendererAPI::Create()
{
	switch (IRendererAPI::GetAPI())
	{
	case IRendererAPI::API::None: LR_CORE_CRITICAL("in ITexture::Create() - RendererAPI::None UNSUPPORTED"); return nullptr;
	case IRendererAPI::API::OpenGL:  return std::make_shared<OpenGLRendererAPI>();
	}

	return nullptr;
}
