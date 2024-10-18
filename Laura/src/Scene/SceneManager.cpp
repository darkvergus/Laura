#include "Scene/SceneManager.h"

namespace Laura
{
	SceneManager::SceneManager()
	{
	}

	SceneManager::~SceneManager()
	{
	}

	std::shared_ptr<RenderableScene> SceneManager::ParseSceneForRendering(std::shared_ptr<Scene> scene, std::shared_ptr<AssetManager> assetManager)
	{
		std::shared_ptr<RenderableScene> rScene = std::make_shared<RenderableScene>();
		rScene->meshesOrBvhsDirty = false;
		rScene->materialsDirty = false;

		rScene->skybox = assetManager->GetTexture(scene->skyboxGUID);

		std::unordered_map<GUID, int> newMeshGuidToIndexMap;
		int meshIdx = 0;
		for (auto entity : scene->Get()->view<TransformComponent>())
		{
			// Are the entities gonna be in the same order? if not then the meshIdxes will be different every frame
			Entity e(entity, scene->Get());
			if (e.HasComponent<MeshComponent>())
			{
				GUID meshGUID = e.GetComponent<MeshComponent>().GetID();
				if (newMeshGuidToIndexMap.find(meshGUID) == newMeshGuidToIndexMap.end())
				{
					newMeshGuidToIndexMap[meshGUID] = meshIdx;
					rScene->uniqueMeshList.push_back(assetManager->GetMesh(meshGUID)); // TODO: This should not copy the mesh
					rScene->uniqueBvhList.push_back(assetManager->GetBVH(meshGUID));
					meshIdx++;
				}
			}
		}

		for (const auto& pair : newMeshGuidToIndexMap)
		{
			if (oldMeshGuidToIndexMap.find(pair.first) == oldMeshGuidToIndexMap.end())
			{
				oldMeshGuidToIndexMap = newMeshGuidToIndexMap;
				rScene->meshesOrBvhsDirty = true;
				break;
			}
		}

		for (auto entity : scene->Get()->view<TransformComponent>())
		{
			Entity e(entity, scene->Get());
			// find the main Camera
			if (e.HasComponent<CameraComponent>())
			{
				if (e.GetComponent<CameraComponent>().isMain)
				{
					rScene->cameraTransform = e.GetComponent<TransformComponent>().GetMatrix();
					rScene->cameraFocalLength = e.GetComponent<CameraComponent>().GetFocalLength();
				}
			}

			if (e.HasComponent<MeshComponent>())
			{
				GUID meshGUID = e.GetComponent<MeshComponent>().GetID();
				rScene->meshAndBvhIndices.push_back(newMeshGuidToIndexMap[meshGUID]); // Connect the meshAndBvhIndices to the uniqueMeshList
			}
		}

		return rScene;
	}
}