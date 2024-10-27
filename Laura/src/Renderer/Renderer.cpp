#include "renderer/Renderer.h"
#include "Assets/TextureLoader.h"
#include "Renderer/BVH/BVHBuilder.h"
#include "GL/glew.h"

namespace Laura 
{
	void Renderer::Init()
	{
		m_Shader = IComputeShader::Create(LR_RESOURCES_PATH "Shaders/RayTracingDefault.comp", glm::uvec3(1));
		m_Shader->Bind();
		
		m_Frame = IImage2D::Create(nullptr, m_FrameResolution.x, m_FrameResolution.y, 0, Image2DType::LR_READ_WRITE);

		m_CameraUBO = IUniformBuffer::Create(80, 0, BufferUsageType::DYNAMIC_DRAW);
		m_RenderSettingsUBO = IUniformBuffer::Create(32, 1, BufferUsageType::DYNAMIC_DRAW);
		m_ObjectsMetadataUBO = IUniformBuffer::Create(16, 2, BufferUsageType::STATIC_DRAW);
		//m_EnvironmentUBO = IUniformBuffer::Create(64, 3, BufferUsageType::DYNAMIC_DRAW);
	}

	void Renderer::SetFrameResolution(const glm::vec2& resolution)
	{
		m_FrameResolution = resolution;
		m_Frame = IImage2D::Create(nullptr, m_FrameResolution.x, m_FrameResolution.y, 0, Image2DType::LR_READ_WRITE);
	}

	void Renderer::SubmitScene(std::shared_ptr<RenderableScene> rScene)
	{
		// this flag determines if the renderer will try to render the scene upon RenderScene() call we still try to update parts which are valid
		m_SceneValid = rScene->isValid; 

		m_AccumulateFrameCount = (!renderSettings.accumulateFrames) ? 0 : m_AccumulateFrameCount++; //TODO: this should not be in the renderer

		m_RenderSettingsUBO->Bind();
		m_RenderSettingsUBO->AddData(0, sizeof(uint32_t), &renderSettings.raysPerPixel);
		m_RenderSettingsUBO->AddData(4, sizeof(uint32_t), &renderSettings.bouncesPerRay);
		m_RenderSettingsUBO->AddData(8, sizeof(uint32_t), &renderSettings.maxAABBIntersections);
		m_RenderSettingsUBO->AddData(12, sizeof(uint32_t), &m_AccumulateFrameCount);
		m_RenderSettingsUBO->AddData(16, sizeof(bool), &renderSettings.displayBVH);
		m_RenderSettingsUBO->Unbind();

		m_CameraUBO->Bind();
		m_CameraUBO->AddData(0, sizeof(glm::mat4), &rScene->cameraTransform);
		m_CameraUBO->AddData(64, sizeof(float), &rScene->cameraFocalLength);
		m_CameraUBO->Unbind();
		
		if (rScene->skyboxDirty)
		{
			m_SkyboxTexture = ITexture2D::Create(rScene->skybox->data, rScene->skybox->width, rScene->skybox->height, 1);
		}

		m_TransformsSSBO = IShaderStorageBuffer::Create(sizeof(glm::mat4) * rScene->transforms.size(), 4, BufferUsageType::DYNAMIC_DRAW);
		m_TransformsSSBO->Bind();
		m_TransformsSSBO->AddData(0, sizeof(glm::mat4) * rScene->transforms.size(), rScene->transforms.data());
		m_TransformsSSBO->Unbind();

		if (rScene->meshesDirty)
		{
			m_ObjectsMetadataUBO->Bind();
			m_ObjectsMetadataUBO->AddData(0, sizeof(uint32_t), &rScene->objectCount);
			m_ObjectsMetadataUBO->Unbind();

			m_ContinuousMeshesSSBO = IShaderStorageBuffer::Create(sizeof(Triangle) * rScene->continuousMeshes.size(), 5, BufferUsageType::STATIC_DRAW);
			m_ContinuousMeshesSSBO->Bind();
			m_ContinuousMeshesSSBO->AddData(0, sizeof(Triangle) * rScene->continuousMeshes.size(), rScene->continuousMeshes.data());
			m_ContinuousMeshesSSBO->Unbind();
			m_MeshMappingsSSBO = IShaderStorageBuffer::Create(sizeof(uint32_t) * rScene->meshMappings.size(), 6, BufferUsageType::STATIC_DRAW);
			m_MeshMappingsSSBO->Bind();
			m_MeshMappingsSSBO->AddData(0, sizeof(uint32_t) * rScene->meshMappings.size(), rScene->meshMappings.data());
			m_MeshMappingsSSBO->Unbind();

			m_ContinuousBvhsSSBO = IShaderStorageBuffer::Create(sizeof(BVH::Node) * rScene->continuousBVHs.size(), 7, BufferUsageType::STATIC_DRAW);
			m_ContinuousBvhsSSBO->Bind();
			m_ContinuousBvhsSSBO->AddData(0, sizeof(BVH::Node) * rScene->continuousBVHs.size(), rScene->continuousBVHs.data());
			m_ContinuousBvhsSSBO->Unbind();
			m_BvhMappingsSSBO = IShaderStorageBuffer::Create(sizeof(uint32_t) * rScene->bvhMappings.size(), 8, BufferUsageType::STATIC_DRAW);
			m_BvhMappingsSSBO->Bind();
			m_BvhMappingsSSBO->AddData(0, sizeof(uint32_t) * rScene->bvhMappings.size(), rScene->bvhMappings.data());
			m_BvhMappingsSSBO->Unbind();
		}
	}

	std::shared_ptr<IImage2D> Renderer::RenderScene()
	{
		if (!m_SceneValid) { return nullptr; }

		m_Shader->Bind();
		m_Shader->setWorkGroupSizes(glm::uvec3(ceil(m_FrameResolution.x / 8),
			                                   ceil(m_FrameResolution.y / 4),
			                                   1));
		m_Shader->Dispatch();
		return m_Frame;
	}

	//void Renderer::UpdateSkyboxUBO(std::shared_ptr<LoadedTexture> skyboxTex)
	//{
	//	m_EnvironmentUBO->Bind();
	//	m_EnvironmentUBO->AddData(0, sizeof(glm::vec3), &skybox.getGroundColor());
	//	m_EnvironmentUBO->AddData(16, sizeof(glm::vec3), &skybox.getHorizonColor());
	//	m_EnvironmentUBO->AddData(32, sizeof(glm::vec3), &skybox.getZenithColor());
	//	bool useGradient = (skybox.getType() == SkyboxType::SKYBOX_GRADIENT) ? true : false;
	//	m_EnvironmentUBO->AddData(48, sizeof(bool), &useGradient);
	//	m_EnvironmentUBO->Unbind();
	//}
}