#pragma once

#include "lrpch.h"
#include "Project/Scene/Scene.h"
#include "Project/Assets/AssetManager.h"
#include "Renderer/IRendererAPI.h"
#include "Renderer/IComputeShader.h"
#include "Renderer/ITexture2D.h"
#include "Renderer/IImage2D.h"
#include "Renderer/IUniformBuffer.h"
#include "Renderer/IShaderStorageBuffer.h"
#include "Core/Profiler.h"
#include <filesystem>

namespace Laura 
{

	class Renderer {
	private:
		struct Settings { /* here are the default ones */
			glm::uvec2 Resolution{ 640, 360 }; // nHD 16:9
			std::filesystem::path ComputeShaderPath = LR_RESOURCES_PATH "Shaders/PathTracing.comp";
			uint32_t raysPerPixel = 1;
			uint32_t bouncesPerRay = 5;
			bool ShouldAccumulate = false;
			bool displayBVH = false;
			uint32_t maxAABBIntersections;
		};

		struct Cache {
			glm::uvec2 Resolution{0};
			std::filesystem::path ActiveShaderPath{};
			uint32_t AccumulatedFrames = 0;
		};

		// Under the std430 - 80 bytes
		struct MeshEntityHandle { 
			MeshEntityHandle(uint32_t firstTriIdx = 0, uint32_t triCount = 0, uint32_t firstNodeIdx = 0, uint32_t nodeCount = 0, glm::mat4 transform = {}) 
			:	FirstTriIdx(firstTriIdx),
				TriCount(triCount),
				FirstNodeIdx(firstNodeIdx),
				NodeCount(nodeCount),
				Transform(transform) {
			}

			glm::mat4 Transform;
			uint32_t FirstTriIdx;
			uint32_t TriCount;
			uint32_t FirstNodeIdx;
			uint32_t NodeCount;
		};

		struct ParsedScene {
			std::vector<MeshEntityHandle> MeshEntityLookupTable; // only renderable entities in the scene

			bool hasValidCamera = false;
			float CameraFocalLength = 0;
			glm::mat4 CameraTransform{};

			LR_GUID skyboxGUID = LR_GUID::INVALID;
		};

	public:
		Renderer(std::shared_ptr<Profiler> profiler)
			: m_Profiler(profiler) {
		};
		~Renderer() = default;

		inline static IRendererAPI::API GetAPI() { return IRendererAPI::GetAPI(); } // getter
		inline static void SetAPI(IRendererAPI::API api) { IRendererAPI::SetAPI(api); } // setter

		void Init();
		std::shared_ptr<IImage2D> Render(const Scene* scene, const AssetPool* resourcePool);

		Settings settings{};

	private:
		std::shared_ptr<const ParsedScene> Parse(const Scene* scene, const AssetPool* resourcePool) const;
		bool SetupGPUResources(std::shared_ptr<const ParsedScene> pScene, const AssetPool* resourcePool);
		void Draw(); // Draws directly to m_Frame

		std::shared_ptr<IComputeShader> m_Shader;
		std::shared_ptr<IImage2D> m_Frame;
		std::shared_ptr<ITexture2D> m_SkyboxTexture;

		std::shared_ptr<IUniformBuffer> m_CameraUBO, m_SettingsUBO;
		std::shared_ptr<IShaderStorageBuffer> m_MeshEntityLookupSSBO, m_MeshBufferSSBO, m_NodeBufferSSBO, m_IndexBufferSSBO;
		
		Cache m_Cache;
		std::shared_ptr<Profiler> m_Profiler;
	};
}