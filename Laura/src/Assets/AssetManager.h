#pragma once 

#include "lrpch.h"
#include "Geometry/Triangle.h"
#include "Renderer/BVH/BVHBuilder.h"

#include "Renderer/Material.h"
#include "Assets/TextureLoader.h" // LoadedTexture struct

#include "Core/GUID.h"


namespace Laura
{
	class AssetManager
	{
	public:
		inline AssetManager()
		{
			m_DefaultMaterial = std::make_shared<Material>();
			m_DefaultMaterial->color = glm::vec3(0.0f, 0.0f, 0.0f);
			m_DefaultMaterial->emissionColor = glm::vec3(1.0f, 0.0f, 0.6f);
			m_DefaultMaterial->emissionStrength = 5.0f;
		}

		LR_GUID LoadTexture(const std::string& filepath, uint32_t channels);
		LR_GUID LoadMesh(const std::string& filepath);
		LR_GUID LoadMaterial(const std::string& filepath);

		void UnloadMesh(LR_GUID id);
		void UnloadTexture(LR_GUID id);
		void UnloadMaterial(LR_GUID id);

		std::shared_ptr<std::vector<Triangle>> GetMesh(LR_GUID id);
		std::shared_ptr<BVH::BVH_data> GetBVH(LR_GUID id);
		std::shared_ptr<Material> GetMaterial(LR_GUID id);
		std::shared_ptr<LoadedTexture> GetTexture(LR_GUID id);

	private:
		std::shared_ptr<Material> m_DefaultMaterial;
		uint32_t HashFilepathToID(const std::string& filepath);

		std::unordered_map<LR_GUID, std::shared_ptr<std::vector<Triangle>>> m_Meshes;
		std::unordered_map<LR_GUID, std::shared_ptr<BVH::BVH_data>> m_BVHs;
		std::unordered_map<LR_GUID, std::shared_ptr<Material>> m_Materials; // key 1 is reserved for default material
		std::unordered_map<LR_GUID, std::shared_ptr<LoadedTexture>> m_Textures;
	};
}