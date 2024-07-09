#include "platform/OpenGL/OpenGLTexture.h"

#include <gl/glew.h>
#include <stb_image/stb_image.h>
#include "platform/OpenGL/OpenGLdebugFuncs.h"


OpenGLTexture::OpenGLTexture(const std::string& filepath, int bindingPoint)
	: m_Filepath(filepath), m_BindingPoint(bindingPoint), m_RendererID(0), m_Width(0), m_Height(0), m_Channels(0) {

	stbi_set_flip_vertically_on_load(1); // flip the image vertically because OpenGL expects the 0,0 coordinate to be at the bottom left corner
	unsigned char* textureData = stbi_load(m_Filepath.c_str(), &m_Width, &m_Height, &m_Channels, 4);
	if (!textureData) {
		LR_CORE_CRITICAL("stbi_load() failed to read the texture. (Did you pass the correct path?): {0}", m_Filepath);
		return;
	}
	GLCall(glActiveTexture(GL_TEXTURE2)); // THE NEXT BOUND TEXTURE will be bound to texture slot 2
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, m_BindingPoint, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData));
	stbi_image_free(textureData);
}

OpenGLTexture::OpenGLTexture(int width, int height, int channels, int bindingPoint)
	: m_Width(width), m_Height(height), m_Channels(channels), m_BindingPoint(bindingPoint), m_RendererID(0), m_Filepath("") {

	// DSA direct state access texture creation (meaning we can create a texture without binding it)
	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTextureStorage2D(m_RendererID, 1, GL_RGBA32F, m_Width, m_Height));
	GLCall(glBindImageTexture(m_BindingPoint, m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
}

OpenGLTexture::~OpenGLTexture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void OpenGLTexture::ChangeBindingPoint(int bindingPoint)
{
	m_BindingPoint = bindingPoint;
	Bind();
}

void OpenGLTexture::Bind()
{
	GLCall(glBindImageTexture(m_BindingPoint, m_RendererID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
}

void OpenGLTexture::Unbind()
{
	GLCall(glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
}