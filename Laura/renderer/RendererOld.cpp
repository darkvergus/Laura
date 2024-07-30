//#include "renderer/RendererOld.h"
//#include "Renderer.h"
//
//struct SceneData;
//
//namespace Laura 
//{
//
//	Renderer::Renderer(SceneData& scene, BVH::BVH_data BVH_of_mesh, std::string& skyboxFilePath)
//		: m_Scene(scene),
//		BVH_of_mesh(BVH_of_mesh),
//		skyboxFilePath(skyboxFilePath)
//	{
//		IRendererAPI::SetAPI(IRendererAPI::API::OpenGL);
//		m_API = IRendererAPI::Create();
//
//		initComputeRtxStage();
//		initComputePostProcStage();
//	}
//
//	Renderer::~Renderer()
//	{
//	}
//
//	void Renderer::setViewportSize(glm::vec2 viewportSize)
//	{
//		GLCall(glViewport(0, 0, viewportSize.x, viewportSize.y));
//		m_ViewportSize = viewportSize;
//		m_TracingTexture = ITexture::Create(viewportSize.x, viewportSize.y, 4, 0);
//		m_PostProcTexture = ITexture::Create(viewportSize.x, viewportSize.y, 4, 1);
//	}
//
//	void Renderer::setSkyboxFilePath(std::string new_skyboxFilePath) {
//		skyboxFilePath = new_skyboxFilePath;
//		m_SkyboxTexture = ITexture::Create(skyboxFilePath, 0);
//	}
//
//
//
//	/////////////////////////
//	/// RAY TRACING STAGE ///
//	void Renderer::initComputeRtxStage()
//	{
//		// I would like to set the texture here but we dont know the texture size yet so we do it in setSize
//		computeRtxShader = IComputeShader::Create(CORE_RESOURCES_PATH "shaders/ComputeRayTracing.comp", glm::uvec3(1));
//		computeRtxShader->Bind();
//		m_rtx_parameters_UBO = IUniformBuffer::Create(188, 0, BufferUsageType::DYNAMIC_DRAW);
//		// unlike rtx parameters ubo since the spheres are STATIC we can set the data HERE already
//		m_sphereBuffer_UBO = IUniformBuffer::Create(m_Scene.size, 1, BufferUsageType::STATIC_DRAW);
//		m_sphereBuffer_UBO->Bind();
//		m_sphereBuffer_UBO->AddData(0, m_Scene.size, m_Scene.sceneObjects);
//		m_sphereBuffer_UBO->Unbind();
//
//		m_tris_SSBO = IShaderStorageBuffer::Create(sizeof(Triangle) * BVH_of_mesh.TRIANGLES_size, 3, BufferUsageType::STATIC_DRAW);
//		m_tris_SSBO->Bind();
//		m_tris_SSBO->AddData(0, sizeof(Triangle) * BVH_of_mesh.TRIANGLES_size, BVH_of_mesh.TRIANGLES.data());
//		m_tris_SSBO->Unbind();
//
//		m_BVH_SSBO = IShaderStorageBuffer::Create(sizeof(BVH::Node) * BVH_of_mesh.BVH_size, 4, BufferUsageType::STATIC_DRAW);
//		m_BVH_SSBO->Bind();
//		m_BVH_SSBO->AddData(0, sizeof(BVH::Node) * BVH_of_mesh.BVH_size, BVH_of_mesh.BVH.data());
//		m_BVH_SSBO->Unbind();
//
//		m_pixelData_SSBO = IShaderStorageBuffer::Create(20, 5, BufferUsageType::DYNAMIC_DRAW);
//	}
//
//	std::shared_ptr<ITexture> Renderer::RenderComputeRtxStage()
//	{
//		m_rtx_parameters_UBO->Bind();
//		m_rtx_parameters_UBO->AddData(0, sizeof(unsigned int), &rtx_uniform_parameters.numAccumulatedFrames);
//		m_rtx_parameters_UBO->AddData(4, sizeof(unsigned int), &rtx_uniform_parameters.raysPerPixel);
//		m_rtx_parameters_UBO->AddData(8, sizeof(unsigned int), &rtx_uniform_parameters.bouncesPerRay);
//		m_rtx_parameters_UBO->AddData(12, sizeof(float), &rtx_uniform_parameters.FocalLength);
//		m_rtx_parameters_UBO->AddData(16, sizeof(glm::vec3), &rtx_uniform_parameters.skyboxGroundColor);
//		m_rtx_parameters_UBO->AddData(32, sizeof(glm::vec3), &rtx_uniform_parameters.skyboxHorizonColor);
//		m_rtx_parameters_UBO->AddData(48, sizeof(glm::vec3), &rtx_uniform_parameters.skyboxZenithColor);
//		m_rtx_parameters_UBO->AddData(64, sizeof(glm::vec3), &rtx_uniform_parameters.CameraPos);
//		m_rtx_parameters_UBO->AddData(80, sizeof(glm::vec3), &rtx_uniform_parameters.pixelGlobalInvocationID);
//		m_rtx_parameters_UBO->AddData(96, sizeof(glm::mat4), &rtx_uniform_parameters.ModelMatrix);
//		m_rtx_parameters_UBO->AddData(160, sizeof(bool), &rtx_uniform_parameters.WasInput);
//		m_rtx_parameters_UBO->AddData(164, sizeof(bool), &rtx_uniform_parameters.display_BVH);
//		m_rtx_parameters_UBO->AddData(168, sizeof(bool), &rtx_uniform_parameters.display_multiple);
//		m_rtx_parameters_UBO->AddData(172, sizeof(unsigned int), &rtx_uniform_parameters.displayed_layer);
//		m_rtx_parameters_UBO->AddData(176, sizeof(unsigned int), &rtx_uniform_parameters.BVH_tree_depth);
//		m_rtx_parameters_UBO->AddData(180, sizeof(bool), &rtx_uniform_parameters.show_skybox);
//		m_rtx_parameters_UBO->AddData(184, sizeof(int), &rtx_uniform_parameters.heatmap_color_limit);
//		m_rtx_parameters_UBO->Unbind();
//
//		computeRtxShader->Bind();
//		computeRtxShader->setWorkGroupSizes(glm::uvec3(ceil(m_ViewportSize.x / 8), ceil(m_ViewportSize.y / 4), 1));
//		computeRtxShader->Dispatch();
//		
//		PixelData* SSBO_pixelData_ptr = (PixelData*)m_pixelData_SSBO->ReadData(0, 20);
//		pixelData.pixelColor = SSBO_pixelData_ptr->pixelColor;
//		pixelData.AABB_intersect_count = SSBO_pixelData_ptr->AABB_intersect_count;
//
//		return m_TracingTexture;
//	}
//	/////////////////////////////
//
//
//
//	/////////////////////////////
//	/// POST PROCESSING STAGE ///
//	void Renderer::initComputePostProcStage()
//	{
//		computePostProcShader = IComputeShader::Create(CORE_RESOURCES_PATH "shaders/ComputePostProcessing.comp", glm::uvec3(1));
//		computePostProcShader->Bind();
//		m_postProcessing_parameters_UBO = IUniformBuffer::Create(4, 2, BufferUsageType::DYNAMIC_DRAW);
//	}
//
//	std::shared_ptr<ITexture> Renderer::RenderComputePostProcStage()
//	{
//		m_postProcessing_parameters_UBO->Bind();
//		m_postProcessing_parameters_UBO->AddData(0, sizeof(unsigned int), &postProcessing_uniform_parameters.numAccumulatedFrames);
//		m_postProcessing_parameters_UBO->Unbind();
//		computePostProcShader->Bind();
//		computePostProcShader->setWorkGroupSizes(glm::uvec3(ceil(m_ViewportSize.x / 8), ceil(m_ViewportSize.y / 4), 1));
//		computePostProcShader->Dispatch();
//		return m_PostProcTexture;
//	}
//	/////////////////////////////
//}
