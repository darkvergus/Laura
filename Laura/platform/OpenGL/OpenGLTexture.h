#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "lrpch.h"
#include "renderer/ITexture.h"

namespace Laura {

	class OpenGLTexture : public ITexture
	{
	public:
		OpenGLTexture(const std::string& filepath, int bindingPoint);
		OpenGLTexture(int width, int height, int channels, int bindingPoint);
		virtual ~OpenGLTexture() override;

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void ChangeBindingPoint(int bindingPoint) override;

		inline virtual int GetWidth()  override { return m_Width; }
		inline virtual int GetHeight() override { return m_Height; }
		inline virtual int GetID() override { return m_ID; }

	private:
		unsigned int m_ID;
		int m_BindingPoint, m_Channels;
		int m_Width, m_Height;
		std::string m_Filepath;
	};

}

#endif // OPENGL_TEXTURE_H
