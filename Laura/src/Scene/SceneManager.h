#pragma once
#include "lrpch.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"

#include "Assets/TextureLoader.h"
#include "Renderer/BVH/BVHBuilder.h"
#include "Assets/AssetManager.h"

namespace Laura
{
	struct RenderableScene
	{
		std::shared_ptr<LoadedTexture> skybox;
		
		glm::mat4 cameraTransform;
		float cameraFocalLength;
	
		// THE IDEA (what the shader does roughly):
		// for index in range(objectCount)
		// based the index, get the starting index of the mesh from meshMappings
		// based the index, get the starting index of the bvh from bvhMappings
		// based the index, get the material index from materialMappings
		
		int objectCount; // number 3D objects being rendered (sent to the shader)
		std::vector<glm::mat4> transforms; // size is object count

		// ALL MAPPINGS ARE OF SIZE OF THE OBJECT COUNT
		std::vector<uint32_t> meshMappings; // contain mappings like 19322, 0, 34212, ... (starting index of the mesh in the continuousMeshes array)
		std::vector<uint32_t> bvhMappings; // contain mappings like 41350, 22310, 0, ... (starting index of the bvh in the continuousBVHs array)
		std::vector<uint32_t> materialMappings; // contain mappings like 0, 1, 4, 3 ... (since the size of a material is constant, we don't need fancy indexes)
		std::vector<BVH::Node> continuousBVHs;
		std::vector<Triangle> continuousMeshes;
		std::vector<Material> continuousMaterials;

		bool skyboxDirty, meshesDirty, materialsDirty, isValid;
	};

	class SceneManager
	{
	public:
		inline SceneManager(std::shared_ptr<AssetManager> assetManager)
			: m_AssetManager(assetManager)
		{
		}
		~SceneManager() = default;
		std::shared_ptr<RenderableScene> ParseSceneForRendering(std::shared_ptr<Scene> scene);
	private:
		std::shared_ptr<AssetManager> m_AssetManager;

		LR_GUID m_Cached_SkyboxGUID;
		std::vector<uint32_t> m_Cached_MeshMappings;
		std::vector<uint32_t> m_Cached_MaterialMappings;
	};
}