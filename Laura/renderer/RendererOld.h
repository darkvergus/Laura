#pragma once
#include "lrpch.h"

// OpenGL dependant (should be removed once refactored)
#include <GL/glew.h>
#include "platform/OpenGL/OpenGLdebugFuncs.h"

#include "CameraOld/CameraOld.h"
#include "Renderer/BVHBuilder.h"

//new stuff
#include "renderer/IRendererAPI.h"
// ------------------------------
#include "renderer/IComputeShader.h"
#include "renderer/IUniformBuffer.h"
#include "renderer/IShaderStorageBuffer.h"
#include "renderer/ITexture.h"


/**
* @brief The SceneData struct
* This struct is used to pass the scene data to the renderer
* */

#ifndef SCENEDATA
#define SCENEDATA
	struct SceneData 
	{
		const void* sceneObjects;
		size_t size;
		int numberOfObjects;
	};
#endif

namespace Laura {

	struct PixelData 
	{
		glm::vec4 pixelColor; // .xyz = color, .w = TRI_intersect_count
		unsigned int AABB_intersect_count;
	};

	struct rtx_parameters_uniform_struct 
	{
		unsigned int numAccumulatedFrames;				// offset 0  // alignment 4 // total 4 bytes
		unsigned int raysPerPixel;						// offset 4  // alignment 4 // total 8 bytes
		unsigned int bouncesPerRay;						// offset 8  // alignment 4 // total 12 bytes
		float FocalLength;								// offset 12 // alignment 4 // total 16 bytes
		glm::vec3 skyboxGroundColor;					// offset 16 // alignment 16 // total 32 bytes
		glm::vec3 skyboxHorizonColor;					// offset 32 // alignment 16 // total 48 bytes
		glm::vec3 skyboxZenithColor;					// offset 48 // alignment 16 // total 64 bytes                            
		glm::vec3 CameraPos;							// offset 64 // alignment 16 // total 80 bytes
		glm::vec3 pixelGlobalInvocationID;				// offset 80 // alignment 16 // total 96 bytes
		glm::mat4 ModelMatrix;							// offset 96 // alignment 16 // total 160 bytes
		bool WasInput;									// offset 160 // alignment 4 // total 164 bytes
		bool display_BVH = display_BVH;					// offset 164 // alignment 4 // total 168 bytes
		bool display_multiple = display_multiple;		// offset 168 // alignment 4 // total 172 bytes
		unsigned int displayed_layer = displayed_layer; // offset 172 // alignment 4 // total 176 bytes
		unsigned int BVH_tree_depth = BVH_tree_depth;	// offset 176 // alignment 4 // total 180 bytes
		bool show_skybox = show_skybox;					// offset 180 // alignment 4 // total 184 bytes
		int heatmap_color_limit = heatmap_color_limit;	// offset 184 // alignment 4 // total 188 bytes
	};

	struct postProcessing_parameters_uniform_struct 
	{
		unsigned int numAccumulatedFrames;
	};


	class Renderer
	{
	public:
		inline static IRendererAPI::API GetAPI() { return IRendererAPI::GetAPI(); }

	public:
		Renderer(SceneData& scene, BVH::BVH_data BVH_of_mesh, std::string& skyboxFilePath);
		~Renderer();

		void setViewportSize(glm::vec2 viewportSize);
		void setSkyboxTexture();
		std::shared_ptr<ITexture> RenderComputeRtxStage();
		rtx_parameters_uniform_struct rtx_uniform_parameters;
		PixelData pixelData; // should be read after rendering the compute rtx stage
		std::shared_ptr<ITexture> RenderComputePostProcStage();
		postProcessing_parameters_uniform_struct postProcessing_uniform_parameters;
		void setSkyboxFilePath(std::string skyboxFilePath);

	private:
		void initComputeRtxStage();
		void initComputePostProcStage();

	private:
		SceneData m_Scene;
		glm::vec2 m_ViewportSize;

		std::shared_ptr<IRendererAPI> m_API;
		std::shared_ptr<IComputeShader> computeRtxShader, computePostProcShader;
		std::shared_ptr<ITexture> m_SkyboxTexture, m_TracingTexture, m_PostProcTexture;
		std::shared_ptr<IUniformBuffer> m_rtx_parameters_UBO, m_sphereBuffer_UBO, m_postProcessing_parameters_UBO;
		std::shared_ptr<IShaderStorageBuffer> m_tris_SSBO, m_BVH_SSBO, m_pixelData_SSBO;

		BVH::BVH_data BVH_of_mesh;
		std::string skyboxFilePath;
	};
}