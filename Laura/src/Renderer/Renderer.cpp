#include "renderer/Renderer.h"
#include <glm/gtc/matrix_access.hpp>

namespace Laura 
{

	void Renderer::Init() {
		m_SettingsUBO = IUniformBuffer::Create(48, 1, BufferUsageType::DYNAMIC_DRAW);
		m_CameraUBO = IUniformBuffer::Create(80, 0, BufferUsageType::DYNAMIC_DRAW);
	}

	std::shared_ptr<IImage2D> Renderer::Render(const Scene* scene, const AssetPool* assetPool) {
		auto t = m_Profiler->timer("Renderer::Render()");

		const auto pScene = Parse(scene, assetPool);
		if (!pScene) { // Most likely scene missing camera
			return nullptr;
		}
		SetupGPUResources(pScene, scene, assetPool);
		Draw();
		return m_Frame;
	}

	std::shared_ptr<const Renderer::ParsedScene> Renderer::Parse(const Scene* scene, const AssetPool* assetPool) const {
		if (scene == nullptr) {
			return nullptr;
		}

		auto t = m_Profiler->timer("Renderer::Parse()");
		auto pScene = std::make_shared<Renderer::ParsedScene>();
	
		// MAIN CAMERA
		auto cameraView = scene->GetRegistry()->view<TransformComponent, CameraComponent>();
		for (auto entity : cameraView) {
			EntityHandle e(entity, scene->GetRegistry());
			if (!e.GetComponent<CameraComponent>().isMain) {
				continue;
			}
			pScene->hasValidCamera = true;
			pScene->CameraTransform = e.GetComponent<TransformComponent>().GetMatrix();
			pScene->CameraFocalLength = e.GetComponent<CameraComponent>().GetFocalLength();
			break;
		}

		if (!pScene->hasValidCamera) {
			return nullptr;
		}

		auto renderableView = scene->GetRegistry()->view<TransformComponent, MeshComponent>();
		pScene->MeshEntityLookupTable.reserve(renderableView.size_hint());
		for (auto entity : renderableView) {
			EntityHandle e(entity, scene->GetRegistry());
			LR_GUID& guid = e.GetComponent<MeshComponent>().guid;
			std::shared_ptr<MeshMetadata> metadata = assetPool->find<MeshMetadata>(guid);
			if (!metadata) {
				continue;
			}
			pScene->MeshEntityLookupTable.emplace_back (
				metadata->firstTriIdx,
				metadata->TriCount,
				metadata->firstNodeIdx,
				metadata->nodeCount,
				e.GetComponent<TransformComponent>().GetMatrix()
			);
		}

		pScene->skyboxGUID = scene->skyboxGuid;
		return pScene;
	}

	// returns false if error occured, else true
	// assumes a valid pScene
	bool Renderer::SetupGPUResources(std::shared_ptr<const ParsedScene> pScene, const Scene* scene, const AssetPool* assetPool) {
		m_Profiler->timer("Renderer::SetupGPUResources()");

		if (settings.ComputeShaderPath != m_Cache.ActiveShaderPath) {
			m_Shader = IComputeShader::Create(settings.ComputeShaderPath.string(), glm::uvec3(1)); // work group sizes set in Draw() before shader->dispatch() 
			if (!m_Shader) {
				return false;
			}
			m_Shader->Bind();
			m_Cache.ActiveShaderPath = settings.ComputeShaderPath;
		}

		if (settings.Resolution != m_Cache.Resolution) {
			m_Frame = IImage2D::Create(nullptr, settings.Resolution.x, settings.Resolution.y, 0, Image2DType::LR_READ_WRITE);
			m_Cache.Resolution = settings.Resolution;
		}

		m_Cache.AccumulatedFrames = (settings.ShouldAccumulate) ? (m_Cache.AccumulatedFrames + 1) : 0;
		{
			// SETTINGS
			uint32_t meshEntityCount = pScene->MeshEntityLookupTable.size();
			uint32_t displayBVH = static_cast<uint32_t>(settings.displayBVH);
			m_SettingsUBO->Bind();
			m_SettingsUBO->AddData(0, sizeof(uint32_t), &settings.raysPerPixel);
			m_SettingsUBO->AddData(4, sizeof(uint32_t), &settings.bouncesPerRay);
			m_SettingsUBO->AddData(8, sizeof(uint32_t), &settings.maxAABBIntersections);
			m_SettingsUBO->AddData(12, sizeof(uint32_t), &m_Cache.AccumulatedFrames);
			m_SettingsUBO->AddData(16, sizeof(uint32_t), &meshEntityCount);
			m_SettingsUBO->AddData(20, sizeof(uint32_t), &displayBVH);
			m_SettingsUBO->Unbind();
		}
		{
			// CAMERA
			m_CameraUBO->Bind();
			m_CameraUBO->AddData(0, sizeof(glm::mat4), &pScene->CameraTransform);
			m_CameraUBO->AddData(64, sizeof(float), &pScene->CameraFocalLength);
			m_CameraUBO->Unbind();
		}
		{
			// ENTITY LOOKUP TABLE - updated every frame (transforms...)
			uint32_t sizeBytes = sizeof(MeshEntityHandle) * pScene->MeshEntityLookupTable.size();
			m_MeshEntityLookupSSBO = IShaderStorageBuffer::Create(sizeBytes, 4, BufferUsageType::DYNAMIC_DRAW);
			m_MeshEntityLookupSSBO->Bind();
			m_MeshEntityLookupSSBO->AddData(0, sizeBytes, pScene->MeshEntityLookupTable.data());
			m_MeshEntityLookupSSBO->Unbind();
		}

		static uint32_t prevMeshBuffVersion = 0;
		static uint32_t prevNodeBuffVersion = 0;
		static uint32_t prevIndexBuffVersion = 0;
		static uint32_t prevSkyboxTextureVersion = 0;

		// Update SKYBOX texture if guid changed 
		if (scene && scene->skyboxGuid != m_Cache.prevSkyboxGuid) {
			m_Cache.prevSkyboxGuid = scene->skyboxGuid;
			auto metadata = assetPool->find<TextureMetadata>(pScene->skyboxGUID);
			if (metadata) {
				const uint32_t SKYBOX_TEXTURE_UNIT = 1;
				const unsigned char* data = &assetPool->TextureBuffer[metadata->texStartIdx];
				m_SkyboxTexture = ITexture2D::Create(data, metadata->width, metadata->height, SKYBOX_TEXTURE_UNIT);
			}
		}

		// MESH BUFFER
		{
    		uint32_t currMeshBuffVersion = assetPool->GetUpdateVersion(AssetPool::AssetType::MeshBuffer);
    		if (prevMeshBuffVersion != currMeshBuffVersion) {
        		prevMeshBuffVersion = currMeshBuffVersion;

        		uint32_t meshBuffer_sizeBytes = sizeof(Triangle) * assetPool->MeshBuffer.size();
        		m_MeshBufferSSBO = IShaderStorageBuffer::Create(meshBuffer_sizeBytes, 5, BufferUsageType::STATIC_DRAW);
        		m_MeshBufferSSBO->Bind();
        		m_MeshBufferSSBO->AddData(0, meshBuffer_sizeBytes, assetPool->MeshBuffer.data());
        		m_MeshBufferSSBO->Unbind();
    		}
		}

		// NODE BUFFER
		{
    		uint32_t currNodeBuffVersion = assetPool->GetUpdateVersion(AssetPool::AssetType::NodeBuffer);
    		if (prevNodeBuffVersion != currNodeBuffVersion) {
        		prevNodeBuffVersion = currNodeBuffVersion;

        		uint32_t nodeBuffer_sizeBytes = sizeof(BVHAccel::Node) * assetPool->NodeBuffer.size();
        		m_NodeBufferSSBO = IShaderStorageBuffer::Create(nodeBuffer_sizeBytes, 6, BufferUsageType::STATIC_DRAW);
        		m_NodeBufferSSBO->Bind();
        		m_NodeBufferSSBO->AddData(0, nodeBuffer_sizeBytes, assetPool->NodeBuffer.data());
        		m_NodeBufferSSBO->Unbind();
    		}
		}

		// INDEX BUFFER
		{
    		uint32_t currIndexBuffVersion = assetPool->GetUpdateVersion(AssetPool::AssetType::IndexBuffer);
    		if (prevIndexBuffVersion != currIndexBuffVersion) {
        		prevIndexBuffVersion = currIndexBuffVersion;

        		uint32_t indexBuffer_sizeBytes = sizeof(uint32_t) * assetPool->IndexBuffer.size();
        		m_IndexBufferSSBO = IShaderStorageBuffer::Create(indexBuffer_sizeBytes, 7, BufferUsageType::STATIC_DRAW);
        		m_IndexBufferSSBO->Bind();
        		m_IndexBufferSSBO->AddData(0, indexBuffer_sizeBytes, assetPool->IndexBuffer.data());
        		m_IndexBufferSSBO->Unbind();
    		}
		}
		return true;
	}

	void Renderer::Draw() {
		auto t = m_Profiler->timer("Renderer::Draw()");
		m_Shader->Bind();
		m_Shader->setWorkGroupSizes(glm::uvec3(
			(settings.Resolution.x + 7) / 8,
			(settings.Resolution.y + 3) / 4,
			1
		  ));
		m_Shader->Dispatch();
	}
}