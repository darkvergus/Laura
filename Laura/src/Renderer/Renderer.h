#pragma once

#include "lrpch.h"

#include "Scene/Scene.h"

#include "Assets/Assets.h"

#include "Renderer/IRendererAPI.h"
#include "Renderer/IComputeShader.h"
#include "Renderer/ITexture2D.h"
#include "Renderer/IImage2D.h"
#include "Renderer/IUniformBuffer.h"
#include "Renderer/IShaderStorageBuffer.h"

#include <filesystem>

namespace Laura 
{

	class Renderer
	{
	private:
		struct Settings /* here are the default ones */
		{
			glm::uvec2 Resolution{ 640, 360 }; // nHD 16:9
			std::filesystem::path ComputeShaderPath = LR_RESOURCES_PATH "Shaders/RayTracingDefault.comp";
			uint32_t raysPerPixel = 1;
			uint32_t bouncesPerRay = 5;
			bool ShouldAccumulate = false;
			bool displayBVH = false;
			uint32_t maxAABBIntersections;
		};

		struct Cache
		{
			glm::uvec2 Resolution{0};
			std::filesystem::path ActiveShaderPath{};
			uint32_t AccumulatedFrames = 0;
		};

		// Under the std430 - 80 bytes
		struct MeshEntityHandle 
		{
			MeshEntityHandle(uint32_t firstTriIdx = 0, uint32_t triCount = 0, uint32_t firstNodeIdx = 0, uint32_t nodeCount = 0, glm::mat4 transform = {}) 
			:	FirstTriIdx(firstTriIdx),
				TriCount(triCount),
				FirstNodeIdx(firstNodeIdx),
				NodeCount(nodeCount),
				Transform(transform)
			{
			}

			uint32_t FirstTriIdx;
			uint32_t TriCount;
			uint32_t FirstNodeIdx;
			uint32_t NodeCount;
			glm::mat4 Transform;
		};

		struct ParsedScene {
			std::vector<MeshEntityHandle> MeshEntityLookupTable;
			uint32_t SkyboxFirstTexIdx = 0, SkyboxWidth = 0, SkyboxHeight = 0, SkyboxChannels = 0;
			bool hasValidCamera = false;
			float CameraFocalLength = 0;
			glm::mat4 CameraTransform{};
		};


	public:
		Renderer() = default;
		~Renderer() = default;

		inline static IRendererAPI::API GetAPI() { return IRendererAPI::GetAPI(); } // getter
		inline static void SetAPI(IRendererAPI::API api) { IRendererAPI::SetAPI(api); } // setter

		inline void Init(){};
		std::shared_ptr<IImage2D> Render(const Scene* scene, const Asset::ResourcePool* resourcePool);

		Settings settings{};

	private:
		std::shared_ptr<const ParsedScene> Parse(const Scene* scene, const Asset::ResourcePool* resourcePool) const;
		bool SetupGPUResources(std::shared_ptr<const ParsedScene> pScene, const Asset::ResourcePool* resourcePool);
		void Draw(); // Draws directly to m_Frame

		std::shared_ptr<IComputeShader> m_Shader;
		std::shared_ptr<IImage2D> m_Frame;
		std::shared_ptr<ITexture2D> m_SkyboxTexture;

		std::shared_ptr<IUniformBuffer> m_CameraUBO, m_SettingsUBO;
		std::shared_ptr<IShaderStorageBuffer> m_MeshEntityLookupSSBO, m_MeshBufferSSBO, m_NodeBufferSSBO;
		
		Cache m_Cache;
	};
}