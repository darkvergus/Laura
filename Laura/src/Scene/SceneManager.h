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

		// updated every frame
		std::vector<glm::mat4> TransformList; 
		
		// updated when meshesOrBvhsDirty
		std::vector<uint64_t> meshAndBvhIndices; 
		std::vector<std::shared_ptr<std::vector<Triangle>>> uniqueMeshList;
		std::vector<std::shared_ptr<BVH::BVH_data>> uniqueBvhList;

		// updated when materialsDirty
		std::vector<uint64_t> materialIndices;
		std::vector<Material> uniqueMaterialList;

		bool meshesOrBvhsDirty, materialsDirty;
	};

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
		std::shared_ptr<RenderableScene> ParseSceneForRendering(std::shared_ptr<Scene> scene, std::shared_ptr<AssetManager> assetManager);
	private:
		std::unordered_map<GUID, int> oldMeshGuidToIndexMap;
	};
}