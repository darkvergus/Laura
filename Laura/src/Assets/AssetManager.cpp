#include "AssetManager.h"

#include "Assets/MeshLoader.h"
#include "Assets/TextureLoader.h"

namespace Laura
{
	uint32_t AssetManager::LoadMesh(const std::string& filepath)
	{
		uint32_t id = HashFilepathToID(filepath);

		if (m_Meshes.find(id) != m_Meshes.end())
		{
			return id;
		}

		auto mesh = MeshLoader::loadMesh(filepath);
		if (mesh)
		{
			m_Meshes[id] = mesh;
			return id;
		}

		return 0;
	}

	uint32_t AssetManager::LoadTexture(const std::string& filepath, uint32_t channels)
	{
		uint32_t id = HashFilepathToID(filepath);

		if (m_Textures.find(id) != m_Textures.end())
		{
			return id;
		}

		LoadedTexture tex = TextureLoader::loadTexture(filepath, channels);

		m_Textures[id] = std::make_shared<LoadedTexture>(tex);
		return id;
	}

	uint32_t AssetManager::LoadMaterial(const std::string& filepath)
	{
		return 0;
	}

	void AssetManager::UnloadTexture(uint32_t id)
	{
		m_Textures.erase(id);
	}

	void AssetManager::UnloadMesh(uint32_t id)
	{
		m_Meshes.erase(id);
	}

	void AssetManager::UnloadMaterial(uint32_t id)
	{
		m_Materials.erase(id);
	}

	std::shared_ptr<std::vector<Triangle>> AssetManager::GetMesh(uint32_t id)
	{
		return m_Meshes.at(id);
	}

	std::shared_ptr<Material> AssetManager::GetMaterial(uint32_t id)
	{
		return m_Materials.at(id);
	}

	std::shared_ptr<LoadedTexture> AssetManager::GetTexture(uint32_t id)
	{
		return m_Textures.at(id);
	}


	uint32_t AssetManager::HashFilepathToID(const std::string& filepath)
	{
		return std::hash<std::string>{}(filepath) + 1; // +1 to reserve 0 for invalid id
	}
}