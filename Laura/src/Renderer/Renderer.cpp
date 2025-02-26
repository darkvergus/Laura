#include "renderer/Renderer.h"

namespace Laura 
{

	std::shared_ptr<IImage2D> Renderer::Render(const Scene* scene, const Asset::ResourcePool* resourcePool)
	{
		const auto pScene = Parse(scene, resourcePool);
		if (!pScene) // Most likely scene missing camera
			return nullptr;
		SetupGPUResources(pScene, resourcePool);
		Draw();

		return m_Frame;
	}


	std::shared_ptr<const Renderer::ParsedScene> Renderer::Parse(const Scene* scene, const Asset::ResourcePool* resourcePool) const
	{
		auto pScene = std::make_shared<Renderer::ParsedScene>();

		auto cameraView = scene->GetRegistry()->view<TransformComponent, CameraComponent>();
		for (auto entity : cameraView) {
			Entity e(entity, scene->GetRegistry());

			if (!e.GetComponent<CameraComponent>().isMain)
				continue;

			pScene->hasValidCamera = true;
			pScene->CameraTransform = e.GetComponent<TransformComponent>().GetMatrix();
			pScene->CameraFocalLength = e.GetComponent<CameraComponent>().GetFocalLength();
			break;
		}

		if (!pScene->hasValidCamera)
			return nullptr;
		
		auto skyboxView = scene->GetRegistry()->view<SkyboxComponent>();
		for (auto entity : skyboxView) {
			Entity e(entity, scene->GetRegistry());
			const auto& skyboxComponent = e.GetComponent<SkyboxComponent>();
			if (!skyboxComponent.isMain)
				continue;

			std::shared_ptr<Asset::TextureMetadata> metadata = resourcePool->Get<Asset::TextureMetadata>(skyboxComponent.guid);
			if (!metadata)
				continue;

			pScene->SkyboxFirstTexIdx = metadata->texStartIdx;
			pScene->SkyboxWidth = metadata->width;
			pScene->SkyboxHeight = metadata->height;
			pScene->SkyboxChannels = metadata->channels;
			break;
		}

		auto renderableView = scene->GetRegistry()->view<TransformComponent, MeshComponent>();
		pScene->MeshEntityLookupTable.reserve(renderableView.size_hint());
		for (auto entity : renderableView) {
			Entity e(entity, scene->GetRegistry());

			LR_GUID& guid = e.GetComponent<MeshComponent>().guid;

			std::shared_ptr<Asset::MeshMetadata> metadata = resourcePool->Get<Asset::MeshMetadata>(guid);
			if (!metadata)
				continue;

			pScene->MeshEntityLookupTable.emplace_back(
				metadata->firstTriIdx,
				metadata->TriCount,
				metadata->firstNodeIdx,
				metadata->nodeCount,
				e.GetComponent<TransformComponent>().GetMatrix()
			);
		}
		return pScene;
	}

	// returns false if error occured, else true
	// assumes a valid pScene
	bool Renderer::SetupGPUResources(std::shared_ptr<const ParsedScene> pScene, const Asset::ResourcePool* resourcePool)
	{
		if (settings.Resolution != m_Cache.Resolution) {
			m_Frame = IImage2D::Create(nullptr, settings.Resolution.x, settings.Resolution.y, 0, Image2DType::LR_READ_WRITE);
			m_Cache.Resolution = settings.Resolution;
		}

		if (settings.ComputeShaderPath != m_Cache.ActiveShaderPath) {
			m_Shader = IComputeShader::Create(settings.ComputeShaderPath.string(), glm::uvec3(1)); // work group sizes set in Draw() before shader->dispatch() 
			if (!m_Shader)
				return false;
			m_Shader->Bind();
			m_Cache.ActiveShaderPath = settings.ComputeShaderPath;
		}

		m_Cache.AccumulatedFrames = (settings.ShouldAccumulate) ? m_Cache.AccumulatedFrames++ : 0;

		{
			// SETTINGS
			m_SettingsUBO = IUniformBuffer::Create(32, 1, BufferUsageType::DYNAMIC_DRAW);
			m_SettingsUBO->Bind();
			m_SettingsUBO->AddData(0, sizeof(uint32_t), &settings.raysPerPixel);
			m_SettingsUBO->AddData(4, sizeof(uint32_t), &settings.bouncesPerRay);
			m_SettingsUBO->AddData(8, sizeof(uint32_t), &settings.maxAABBIntersections);
			m_SettingsUBO->AddData(12, sizeof(uint32_t), &m_Cache.AccumulatedFrames);
			m_SettingsUBO->AddData(16, sizeof(bool), &settings.displayBVH);
			m_SettingsUBO->Unbind();
		}
		{
			// CAMERA
			m_CameraUBO = IUniformBuffer::Create(80, 0, BufferUsageType::DYNAMIC_DRAW);
			m_CameraUBO->Bind();
			m_CameraUBO->AddData(0, sizeof(glm::mat4), &pScene->CameraTransform);
			m_CameraUBO->AddData(64, sizeof(float), &pScene->CameraFocalLength);
			m_CameraUBO->Unbind();
		}
		{
			// SKYBOX
			const unsigned char* data = &resourcePool->TextureBuffer[pScene->SkyboxFirstTexIdx];
			m_SkyboxTexture = ITexture2D::Create(data, pScene->SkyboxWidth, pScene->SkyboxHeight, 1); // TODO: for some reason Channels are not passed
		}
		{
			// ENTITY LOOKUP TABLE
			uint32_t sizeBytes = sizeof(MeshEntityHandle) * pScene->MeshEntityLookupTable.size();
			m_MeshEntityLookupSSBO = IShaderStorageBuffer::Create(sizeBytes, 4, BufferUsageType::DYNAMIC_DRAW);
			m_MeshEntityLookupSSBO->Bind();
			m_MeshEntityLookupSSBO->AddData(0, sizeBytes, pScene->MeshEntityLookupTable.data());
			m_MeshEntityLookupSSBO->Unbind();
		}
		{
			// RESOURCE POOL 
			uint32_t meshBuffer_sizeBytes = sizeof(Asset::Triangle) * resourcePool->MeshBuffer.size();
			m_MeshBufferSSBO = IShaderStorageBuffer::Create(meshBuffer_sizeBytes, 5, BufferUsageType::STATIC_DRAW);
			m_MeshBufferSSBO->Bind();
			m_MeshBufferSSBO->AddData(0, meshBuffer_sizeBytes, resourcePool->MeshBuffer.data());
			m_MeshBufferSSBO->Unbind();

			uint32_t nodeBuffer_sizeBytes = sizeof(Asset::BVHAccel::Node) * resourcePool->NodeBuffer.size();
			m_NodeBufferSSBO = IShaderStorageBuffer::Create(nodeBuffer_sizeBytes, 6, BufferUsageType::STATIC_DRAW);
			m_NodeBufferSSBO->Bind();
			m_NodeBufferSSBO->AddData(0, nodeBuffer_sizeBytes, resourcePool->NodeBuffer.data());
			m_NodeBufferSSBO->Unbind();
		}
	}

	void Renderer::Draw()
	{
		m_Shader->Bind();
		m_Shader->setWorkGroupSizes(
			glm::uvec3(ceil(settings.Resolution.x / 8), ceil(settings.Resolution.y / 4), 1)
		);
		m_Shader->Dispatch();
	}
}