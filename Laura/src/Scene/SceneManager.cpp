#include "Scene/SceneManager.h"

namespace Laura
{
	std::shared_ptr<RenderableScene> SceneManager::ParseSceneForRendering(std::shared_ptr<Scene> scene)
	{
		std::shared_ptr<RenderableScene> rScene = std::make_shared<RenderableScene>();
		rScene->isValid = false;
		/// SKYBOX /////////////////////////////////////////////////////////////////////////
		rScene->skyboxDirty = false;
		if (m_Cached_SkyboxGUID != scene->skyboxGuid)
		{
			// if the GUID hasn't changed the renderer won't need the texture
			rScene->skybox = m_AssetManager->GetTexture(scene->skyboxGuid);
			rScene->skyboxDirty = true;
		}
		m_Cached_SkyboxGUID = scene->skyboxGuid;

		/// GENERATE ENTITY LIST AND UNIQUE MESH/MATERIAL GUID SETS ////////////////////////
		std::vector<Entity> entityList;
		std::unordered_set<GUID> meshGuidSet;
		std::unordered_set<GUID> materialGuidSet;
		for (auto enttEntity : scene->GetRegistry()->view<TransformComponent>())
		{
			Entity e(enttEntity, scene->GetRegistry());
			if (e.HasComponent<CameraComponent>())
			{
				CameraComponent camera = e.GetComponent<CameraComponent>();
				if (camera.isMain)
				{
					rScene->isValid = true;
					rScene->cameraTransform = e.GetComponent<TransformComponent>().GetMatrix();
					rScene->cameraFocalLength = camera.GetFocalLength();
				}
			}

			// every renderable entity has to have a transform and a mesh component | materials are optional i.e. default material is assigned if not present
			if (e.HasComponent<MeshComponent>())
			{
				if (e.GetComponent<GUIDComponent>().guid == 0)
					continue;

				GUID meshGuid = e.GetComponent<MeshComponent>().guid;
				if (meshGuid == 0)
					continue;
				meshGuidSet.insert(meshGuid);

				if (e.HasComponent<MaterialComponent>())
				{
					GUID materialGuid = e.GetComponent<MaterialComponent>().guid;
					materialGuidSet.insert(materialGuid); // 0 = default material else custom material
				}
				else
				{
					materialGuidSet.insert(0); // default material
				}

				entityList.push_back(e); // this entity is valid for rendering so we add it to the entity array
			}
		}
		rScene->objectCount = entityList.size();

		/// SORTING ///////////////////////////////////////////////////////////////////////
		// After collecting the entities and mesh GUIDs, we sort them to ensure a stable, predictable order.
		// Mesh GUIDs are sorted, and entities are sorted by their GUID to ensure consistency in rendering order and lookups.
		std::vector<GUID> meshGuidList(meshGuidSet.begin(), meshGuidSet.end());
		std::vector<GUID> materialGuidList(materialGuidSet.begin(), materialGuidSet.end());
		std::sort(meshGuidList.begin(), meshGuidList.end());
		std::sort(materialGuidList.begin(), materialGuidList.end());
		std::sort(entityList.begin(), entityList.end(), 
			[](const Entity& e1, const Entity& e2) 
			{
				return e1.GetComponent<GUIDComponent>().guid < e2.GetComponent<GUIDComponent>().guid;
			});


		std::vector<uint32_t> meshMappingsIndexed;
		/// SET INDEX MAPPINGS ////////////////////////////////////////////////////////////
		for (Entity e : entityList)
		{
			auto meshIterator = std::lower_bound(meshGuidList.begin(), meshGuidList.end(), e.GetComponent<MeshComponent>().guid);
			uint32_t meshIndex = std::distance(meshGuidList.begin(), meshIterator);
			meshMappingsIndexed.push_back(meshIndex);

			auto materialIterator = std::lower_bound(materialGuidList.begin(), materialGuidList.end(), e.GetComponent<MaterialComponent>().guid);
			uint32_t materialIndex = std::distance(materialGuidList.begin(), materialIterator);
			rScene->materialMappings.push_back(materialIndex);

			rScene->transforms.push_back(e.GetComponent<TransformComponent>().GetMatrix());
		}

		std::vector<uint32_t> meshIndicesSized;
		std::vector<uint32_t> bvhIndicesSized;
		uint32_t prevMeshIndexSized = 0, prevBvhIndexSized = 0;
		for (GUID guid : meshGuidList)
		{
			std::shared_ptr<std::vector<Triangle>> mesh = m_AssetManager->GetMesh(guid);
			rScene->continuousMeshes.insert(rScene->continuousMeshes.end(), mesh->begin(), mesh->end());
			meshIndicesSized.push_back(prevMeshIndexSized);
			prevMeshIndexSized += mesh->size();

			std::shared_ptr<BVH::BVH_data> meshBVH = m_AssetManager->GetBVH(guid);
			rScene->continuousBVHs.insert(rScene->continuousBVHs.end(), meshBVH->BVH.begin(), meshBVH->BVH.end());
			bvhIndicesSized.push_back(prevBvhIndexSized);
			prevBvhIndexSized += meshBVH->BVH_size;
		}
		for (size_t i = 0; i < meshMappingsIndexed.size(); i++)
		{
			rScene->bvhMappings.push_back(bvhIndicesSized[meshMappingsIndexed[i]]);
			rScene->meshMappings.push_back(meshIndicesSized[meshMappingsIndexed[i]]);
		}

		for (GUID guid : materialGuidList)
		{
			std::shared_ptr<Material> material = m_AssetManager->GetMaterial(guid);
			rScene->continuousMaterials.push_back(*material);
		}

		/// CHECK FLAGS ////////////////////////////////////////////////////////////////////
		rScene->meshesDirty = false;
		rScene->materialsDirty = false;

		for (size_t i = 0; i < rScene->meshMappings.size(); i++)
		{
			if (i >= m_Cached_MeshMappings.size() || m_Cached_MeshMappings[i] != rScene->meshMappings[i])
			{
				rScene->meshesDirty = true;
				break;
			}
		}
		for (size_t i = 0; i < rScene->materialMappings.size(); i++)
		{
			if (i >= m_Cached_MaterialMappings.size() || m_Cached_MaterialMappings[i] != rScene->materialMappings[i])
			{
				rScene->materialsDirty = true;
				break;
			}
		}
		
		/// CACHE DATA FOR THE NEXT FRAME /////////////////////////////////////////////////
		m_Cached_MeshMappings = rScene->meshMappings;
		m_Cached_MaterialMappings = rScene->materialMappings;

		return rScene;
	}
}