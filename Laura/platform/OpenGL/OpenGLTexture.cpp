#include "platform/OpenGL/OpenGLTexture.h"

#include <gl/glew.h>
#include <stb_image/stb_image.h>
#include "platform/OpenGL/OpenGLdebugFuncs.h"

namespace Laura {

	OpenGLTexture::OpenGLTexture(const std::string& filepath, int textureUnitSlot)
		: m_Filepath(filepath), m_TextureUnitSlot(textureUnitSlot), m_ID(0), m_Width(0), m_Height(0), m_Channels(0) {

		stbi_set_flip_vertically_on_load(1); // flip the image vertically because OpenGL expects the 0,0 coordinate to be at the bottom left corner
		unsigned char* textureData = stbi_load(m_Filepath.c_str(), &m_Width, &m_Height, &m_Channels, 4);
		if (!textureData) {
			LR_CORE_CRITICAL("stbi_load() failed to read the texture. (Did you pass the correct path?): {0}", m_Filepath);
			return;
		}
		GLCall(glActiveTexture(GL_TEXTURE0 + m_TextureUnitSlot)); // THE NEXT BOUND TEXTURE will be bound to texture slot 2
		GLCall(glGenTextures(1, &m_ID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData));
		stbi_image_free(textureData);
	}

	OpenGLTexture::OpenGLTexture(int width, int height, int channels, int textureUnitSlot)
		: m_Width(width), m_Height(height), m_Channels(channels), m_TextureUnitSlot(textureUnitSlot), m_ID(0), m_Filepath("") {

		// DSA direct state access texture creation (meaning we can create a texture without binding it)
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ID));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTextureStorage2D(m_ID, 1, GL_RGBA32F, m_Width, m_Height));
		GLCall(glBindImageTexture(m_TextureUnitSlot, m_ID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
	}

	OpenGLTexture::~OpenGLTexture()
	{
		GLCall(glDeleteTextures(1, &m_ID));
	}

	void OpenGLTexture::ChangeTextureUnitSlot(int textureUnitSlot)
	{
		if (textureUnitSlot < 0 || textureUnitSlot >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) 
		{
			LR_CORE_CRITICAL("Error: Invalid texture unit slot {0}", textureUnitSlot);
			return;
		}

		m_TextureUnitSlot = textureUnitSlot;
		GLCall(glActiveTexture(GL_TEXTURE0 + m_TextureUnitSlot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
	}
}