#include "renderer/Renderer.h"
#include "Renderer/BVH/BVHBuilder.h"

namespace Laura 
{
	Renderer::Renderer()
		: m_AccumulateFrameCount(0)
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::BeginScene(const Camera& camera, const EnvironmentEntity& environment)
	{
		m_FrameTexture = ITexture::Create(renderSettings.viewportDimensions.x, renderSettings.viewportDimensions.y, 4, 0);
		//m_SkyboxTexture = ITexture::Create(environment.skybox.getTexturePath(), 1);
		m_SkyboxTexture = ITexture::Create(std::string(APP_RESOURCES_PATH "skyboxes/metro_noord_4k.hdr"), 1);
		m_CameraUBO = IUniformBuffer::Create(80, 0, BufferUsageType::DYNAMIC_DRAW);
		m_EnvironmentUBO = IUniformBuffer::Create(64, 1, BufferUsageType::DYNAMIC_DRAW);
		m_RenderSettingsUBO = IUniformBuffer::Create(32, 2, BufferUsageType::DYNAMIC_DRAW);
		
		m_Shader = IComputeShader::Create(CORE_RESOURCES_PATH "shaders/RayTracingDefault.comp", glm::uvec3(1));
		// could potentially split the BVH rendering mode to another shader altogether
		m_Shader->Bind();

		UpdateCamera(camera);
		UpdateEnvironment(environment);
		UpdateRenderSettings();
	}

	void Renderer::UpdateCamera(const Camera& camera)
	{
		m_CameraUBO->Bind();
		m_CameraUBO->AddData(0, sizeof(glm::mat4), &camera.transform.toMatrix());
		float focalLength = camera.getFocalLength();
		m_CameraUBO->AddData(64, sizeof(float), &focalLength);
		m_CameraUBO->Unbind();
	}

	void Renderer::UpdateEnvironment(const EnvironmentEntity& environment)
	{
		m_EnvironmentUBO->Bind();
		m_EnvironmentUBO->AddData(0, sizeof(glm::vec3), &environment.skybox.getGroundColor());
		m_EnvironmentUBO->AddData(16, sizeof(glm::vec3), &environment.skybox.getHorizonColor());
		m_EnvironmentUBO->AddData(32, sizeof(glm::vec3), &environment.skybox.getZenithColor());
		bool useGradient = (environment.skybox.getType() == SkyboxType::SKYBOX_GRADIENT) ? true : false;
		m_EnvironmentUBO->AddData(48, sizeof(bool), &useGradient);
		m_EnvironmentUBO->Unbind();
	}

	void Renderer::UpdateRenderSettings()
	{
		m_RenderSettingsUBO->Bind();
		m_RenderSettingsUBO->AddData(0, sizeof(uint32_t), &renderSettings.raysPerPixel);
		m_RenderSettingsUBO->AddData(4, sizeof(uint32_t), &renderSettings.bouncesPerRay);
		m_RenderSettingsUBO->AddData(8, sizeof(uint32_t), &renderSettings.maxAABBIntersections);
		m_AccumulateFrameCount = (!renderSettings.accumulateFrames) ? 0 : m_AccumulateFrameCount++;
		m_RenderSettingsUBO->AddData(12, sizeof(uint32_t), &m_AccumulateFrameCount);
		m_RenderSettingsUBO->AddData(16, sizeof(bool), &renderSettings.displayBVH);
		m_RenderSettingsUBO->Unbind();
	}

	void Renderer::SubmitMesh(MeshComponent& mesh)
	{
		BVH::BVH_data meshBVH = BVH::construct(mesh, BVH::Heuristic::SURFACE_AREA_HEURISTIC_BUCKETS);

		m_TriangleMeshSSBO = IShaderStorageBuffer::Create(sizeof(Triangle) * meshBVH.TRIANGLES_size, 3, BufferUsageType::STATIC_DRAW);
		m_TriangleMeshSSBO->Bind();
		m_TriangleMeshSSBO->AddData(0, sizeof(Triangle) * meshBVH.TRIANGLES_size, meshBVH.TRIANGLES.data());
		m_TriangleMeshSSBO->Unbind();

		m_BVHSSBO = IShaderStorageBuffer::Create(sizeof(BVH::Node) * meshBVH.BVH_size, 4, BufferUsageType::STATIC_DRAW);
		m_BVHSSBO->Bind();
		m_BVHSSBO->AddData(0, sizeof(BVH::Node) * meshBVH.BVH_size, meshBVH.BVH.data());
		m_BVHSSBO->Unbind();
	}

	//void Renderer::RemoveMesh(uint32_t meshID)
	//{
	//}
	//
	//void Renderer::SubmitSphere(const SphereComponent& sphere, uint32_t sphereID)
	//{
	//}
	//
	//void Renderer::RemoveSphere(uint32_t sphereID)
	//{
	//}
	//
	//void Renderer::SubmitPlane(const PlaneComponent& plane, uint32_t planeID)
	//{
	//}
	//
	//void Renderer::RemovePlane(uint32_t planeID)
	//{
	//}

	std::shared_ptr<ITexture> Renderer::RenderScene()
	{
		m_Shader->Bind();
		m_Shader->setWorkGroupSizes(glm::uvec3(ceil(renderSettings.viewportDimensions.x / 8),
			                                   ceil(renderSettings.viewportDimensions.y / 4), 
			                                   1));
		m_Shader->Dispatch();

		return m_FrameTexture;
	}

	void Renderer::EndScene()
	{
	}
}