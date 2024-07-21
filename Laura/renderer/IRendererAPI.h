#ifndef IRENDERER_API_H
#define IRENDERER_API_H

#include "lrpch.h"
#include <glm/glm.hpp>

#include "renderer/IComputeShader.h"

namespace Laura {

	class IRendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
			//Vulkan = 2, // !!! Not IMPLEMENTED YET !!!
		};
	public:
		virtual void Init() = 0;
		virtual void Clear(const glm::vec4& color) = 0;
		virtual void SetViewportSize(uint32_t width, uint32_t height) = 0;
		virtual void Dispatch(std::shared_ptr<IComputeShader> computeShader) = 0;

		static std::shared_ptr<IRendererAPI> Create();
		static API GetAPI() { return s_API; }
		static void SetAPI(API api) { s_API = api; }

	private:
		static API s_API;
	};

}

#endif // IRENDERER_API_H