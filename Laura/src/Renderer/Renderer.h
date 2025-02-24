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

namespace Laura 
{

	class Renderer
	{
	private:
		struct Settings
		{
			uint32_t raysPerPixel;
			uint32_t bouncesPerRay;
			bool accumulateFrames;
			bool displayBVH;
			uint32_t maxAABBIntersections;
			glm::vec2 resolution;
		};

		struct EntityHandle 
		{
			uint32_t FirstTriIdx	= 0;
			uint32_t TriCount		= 0;
			uint32_t FirstNodeIdx	= 0;
			uint32_t NodeCount		= 0;
			glm::mat4 transform;
		};

		struct ParsedScene {
			std::vector<EntityHandle> entityLookupTable;
			uint32_t FirstTexByteSkyboxIdx = 0;
			uint32_t SkyboxTexByteCount	   = 0;
			glm::mat4 CameraTransform;
			float CameraFocalLength;
			bool hasValidCamera			   = false;
		};


	public:
		Renderer() = default;
		~Renderer() = default;

		inline static IRendererAPI::API GetAPI() { return IRendererAPI::GetAPI(); } // getter
		inline static void SetAPI(IRendererAPI::API api) { IRendererAPI::SetAPI(api); } // setter

		void Init();
		std::shared_ptr<IImage2D> Render(Scene* scene, Asset::ResourcePool* resourcePool, Settings& renderSettings);

	private:
		std::shared_ptr<ParsedScene> Parse(Scene* scene, Asset::ResourcePool* resourcePool);
		std::shared_ptr<IImage2D> Draw();

		std::shared_ptr<IComputeShader> m_Shader;
		std::shared_ptr<ITexture2D> m_SkyboxTexture;
		std::shared_ptr<IImage2D> m_Frame;
		std::shared_ptr<IUniformBuffer> m_CameraUBO, m_RenderSettingsUBO, m_EnvironmentUBO, m_ObjectsMetadataUBO;
		std::shared_ptr<IShaderStorageBuffer> m_TransformsSSBO, m_ContinuousMeshesSSBO, m_ContinuousBvhsSSBO, m_MeshMappingsSSBO, m_MeshSizesSSBO, m_BvhMappingsSSBO, m_BVHSizesSSBO;

		uint32_t m_AccumulateFrameCount;
	};
}