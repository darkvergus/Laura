#pragma once 

#include "lrpch.h"
#include "Geometry/Triangle.h"
#include "Renderer/Material.h"
#include "Assets/TextureLoader.h" // LoadedTexture struct

namespace Laura
{
	class AssetManager
	{
	public:
		uint32_t LoadTexture(const std::string& filepath, uint32_t channels);
		uint32_t LoadMesh(const std::string& filepath);
		uint32_t LoadMaterial(const std::string& filepath);

		void UnloadTexture(uint32_t id);
		void UnloadMesh(uint32_t id);
		void UnloadMaterial(uint32_t id);

		std::shared_ptr<std::vector<Triangle>> GetMesh(uint32_t id);
		std::shared_ptr<Material> GetMaterial(uint32_t id);
		std::shared_ptr<LoadedTexture> GetTexture(uint32_t id);

	private:
		uint32_t HashFilepathToID(const std::string& filepath);

		std::unordered_map<uint32_t, std::shared_ptr<std::vector<Triangle>>> m_Meshes;
		std::unordered_map<uint32_t, std::shared_ptr<Material>> m_Materials;
		std::unordered_map<uint32_t, std::shared_ptr<LoadedTexture>> m_Textures;
	};
}