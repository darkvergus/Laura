#ifndef OPENGL_RENDERER_API_H
#define OPENGL_RENDERER_API_H

#include "lrpch.h"
#include "renderer/IRendererAPI.h"

class OpenGLRendererAPI : public IRendererAPI
{
public:
	virtual void Init() override;
	virtual void Clear(const glm::vec4& color) override;
	virtual void SetViewportSize(uint32_t width, uint32_t height) override;
	virtual void Dispatch(std::shared_ptr<IComputeShader> computeShader) override;
};


#endif // OPENGL_RENDERER_API_H