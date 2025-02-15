#include "AssetManager.h"

#include "Assets/MeshLoader.h"
#include "Assets/TextureLoader.h"

#include <cassert>

namespace Laura
{
	LR_GUID AssetManager::LoadMesh(const std::string& filepath)
	{
		auto mesh = MeshLoader::loadMesh(filepath);
		if (mesh)
		{
			LR_GUID id;
			m_Meshes[id] = mesh;
			BVH::BVH_data meshBVH = BVH::construct(mesh, BVH::Heuristic::SURFACE_AREA_HEURISTIC_BUCKETS);
			m_BVHs[id] = std::make_shared<BVH::BVH_data>(meshBVH);
			return id;
		}

		return LR_GUID(0); // invalid id
	}

	LR_GUID AssetManager::LoadTexture(const std::string& filepath, uint32_t channels)
	{
		LR_GUID id;
		LoadedTexture tex = TextureLoader::loadTexture(filepath, channels);

		m_Textures[id] = std::make_shared<LoadedTexture>(tex);
		return id;
	}

	LR_GUID AssetManager::LoadMaterial(const std::string& filepath)
	{
		// TODO: Implement
		return 0;
	}

	void AssetManager::UnloadTexture(LR_GUID id)
	{
		m_Textures.erase(id);
	}

	void AssetManager::UnloadMesh(LR_GUID id)
	{
		m_Meshes.erase(id);
		m_BVHs.erase(id);
	}

	void AssetManager::UnloadMaterial(LR_GUID id)
	{
		m_Materials.erase(id);
	}

	std::shared_ptr<std::vector<Triangle>> AssetManager::GetMesh(LR_GUID id)
	{
		auto it = m_Meshes.find(id);
		assert(it != m_Meshes.end() && "Invalid BVH GUID");
		return it->second;
	}

	std::shared_ptr<BVH::BVH_data> AssetManager::GetBVH(LR_GUID id)
	{
		auto it = m_BVHs.find(id);
		assert(it != m_BVHs.end() && "Invalid BVH GUID");
		return it->second;
	}

	std::shared_ptr<Material> AssetManager::GetMaterial(LR_GUID id)
	{
		if (id == 0) 
		{
			return m_DefaultMaterial;
		}
		auto it = m_Materials.find(id);
		assert(it != m_Materials.end() && "Invalid BVH GUID");
		return it->second;
	}

	std::shared_ptr<LoadedTexture> AssetManager::GetTexture(LR_GUID id)
	{
		auto it = m_Textures.find(id);
		assert(it != m_Textures.end() && "Invalid BVH GUID");
		return it->second;
	}


	uint32_t AssetManager::HashFilepathToID(const std::string& filepath)
	{
		return std::hash<std::string>{}(filepath) + 1; // +1 to reserve 0 for invalid id
	}
}