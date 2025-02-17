#pragma once

#include "lrpch.h"

#include "Core/GUID.h"

#include "Assets/BVHAccel.h"

#include <filesystem>

namespace Laura::Asset
{
	struct Metadata{
		virtual ~Metadata() = default;
	};

	struct MeshMetadata : public Metadata {
		uint32_t vertStartIdx = 0; // also bvhIndexBufferStartIdx
		uint32_t vertSize = 0;	   // also bvhIndexBufferSize
		uint32_t bvhStartIdx = 0;
		uint32_t bvhSize = 0;
		std::filesystem::path path = {};

		~MeshMetadata() override = default;
	};

	struct TextureMetadata : public Metadata {
		uint32_t texStartIdx = 0;
		int32_t width = 0;
		int32_t height = 0;
		int32_t channels = 0;
		std::filesystem::path path = {};

		~TextureMetadata() override = default;
	};

	typedef glm::vec3 Vertex; // will expand in the future

	struct ResourcePool {
		std::unordered_map<LR_GUID, std::shared_ptr<Metadata>> metadata;
		std::vector<Vertex> meshVerts;
		std::vector<BVHAccel::Node> bvhNodes;
		std::vector<uint32_t> bvhIndexBuffer; // indirection between BVHAccel::Node and vertices in ResourcePool::meshVerts
		std::vector<unsigned char> textureData;
	};

	class Manager
	{
	public:
		Manager(ResourcePool* pool = nullptr)
			: resourcePool(pool) 
		{
		}
		~Manager() = default;
		
		inline void SetResourcePool(ResourcePool* pool) {
			resourcePool = pool;
		}

		LR_GUID LoadMesh(const std::filesystem::path& path);
		// 0: original (default), 1: grayscale, 2: grayscale+alpha, 3: RGB, 4: RGBA
		LR_GUID LoadTexture(const std::filesystem::path& path, const int channels = 0);

		template <typename T>
		std::shared_ptr<T> Get(const LR_GUID& guid) {
			if (!ResourcePoolValid(resourcePool))
				return nullptr;
			auto it = resourcePool->metadata.find(guid);
			if (it == resourcePool->metadata.end())
				return nullptr;
			return std::dynamic_pointer_cast<T>(it->second);
		}

		ResourcePool* resourcePool = nullptr;
	
	private:
		static inline bool ResourcePoolValid(ResourcePool* resourcePool) {
			if (!resourcePool)
				LR_CORE_WARN("Asset::ResourcePool* in Asset::Manager is nullptr");
		}
	};
}