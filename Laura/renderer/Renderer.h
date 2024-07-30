#ifndef RENDERER_H
#define RENDERER_H

#include "lrpch.h"

#include "Components/Camera.h"
#include "Components/MeshComponent.h"
#include "Entity/Environment.h"

#include "Renderer/IRendererAPI.h"
#include "Renderer/IComputeShader.h"
#include "Renderer/ITexture.h"
#include "Renderer/IUniformBuffer.h"
#include "Renderer/IShaderStorageBuffer.h"

namespace Laura {



	class Renderer
	{
	private:
		struct RenderSettings
		{
			glm::vec2 viewportDimensions;
			uint32_t raysPerPixel;
			uint32_t bouncesPerRay;
			bool accumulateFrames;
			bool displayBVH;
			uint32_t maxAABBIntersections;
		};
	public:
		Renderer();
		~Renderer();

		inline static IRendererAPI::API GetAPI() { return IRendererAPI::GetAPI(); }
		inline static void SetAPI(IRendererAPI::API api) { IRendererAPI::SetAPI(api); }

		// 1. load the camera information to a UBO with binding point 0
		// 2. load the environment information to a UBO with a binding point 1
		// 3. load the rendererSettings information to a UBO with a binding point 2
		// 4. load the default shader
		// 5. load the default texture
		void BeginScene(const Camera& camera, const EnvironmentEntity& environment);

		void UpdateCamera(const Camera& camera);
		void UpdateEnvironment(const EnvironmentEntity& environment);
		
		void UpdateRenderSettings();
		RenderSettings renderSettings;

		// submit mesh has 3 roles
		// 1. save the mesh data to some kind of data sturcture
		// 2. based on searching the data structure set flag genBottomLevelBVH = true/false - of course if this flag is false then the following flag will have to be false as well
		// 3. if the transform has changed set flag genTopLevelBVH = true/false
		// (not gonna be called every frame, only when the mesh is added)
		void SubmitMesh(MeshComponent& mesh);//, uint32_t meshID); No MESH ID for now
		// removes the mesh from the data structure and subsequently sets the genTLBVH=true
		// (also not gonna be called every frame, only when the mesh is removed)
		//void RemoveMesh(uint32_t meshID);

		//void SubmitSphere(const SphereComponent& sphere, uint32_t sphereID);
		//void RemoveSphere(uint32_t sphereID);

		//void SubmitPlane(const PlaneComponent& plane, uint32_t planeID);
		//void RemovePlane(uint32_t planeID);

		// 1. with each ID will be associated the mesh and the BVH flags
		// 2. loop over the data structure and for each mesh that has genBLBVH=true generate the BLBVH
		// 3. if any of the meshes have genTLBVH=true, generate the TLBVH as well
		// 4. send the BVH and meshes to the default RayTracing shader to predetermined binding points
		// 5. dispatch the shader and return the rendered frame
		std::shared_ptr<ITexture> RenderScene();

		// 1. delete the camera, environment and renderSettings UBOs
		// 2. delete the default shader
		// 3. delete the default texture
		// 4. release any memory that was allocated
		void EndScene();

	private:
		std::shared_ptr<IComputeShader> m_Shader;
		std::shared_ptr<ITexture> m_FrameTexture, m_SkyboxTexture;
		std::shared_ptr<IUniformBuffer> m_CameraUBO, m_RenderSettingsUBO, m_EnvironmentUBO;
		std::shared_ptr<IShaderStorageBuffer> m_TriangleMeshSSBO, m_BVHSSBO;

	private:
		uint32_t m_AccumulateFrameCount;
		bool m_genTopLevelBVH, m_genBottomLevelBVH;
	};

}

#endif // RENDERER_H