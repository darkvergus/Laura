#pragma once

#include "lrpch.h"

#include "Assets/AssetTypes.h"
#include "Assets/BVHAccel.h"

#include "Core/GUID.h"

#include <filesystem>

constexpr const char* SUPPORTED_MESH_FILE_FORMATS[]		= { ".fbx", ".obj" ,".gltf", ".glb" };
constexpr const char* SUPPORTED_TEXTURE_FILE_FORMATS[]	= { ".png", ".jpg", ".jpeg", ".tga", ".bpm", ".hdr" };

namespace Laura::Asset
{
	struct ResourcePool {
		std::unordered_map<LR_GUID, std::shared_ptr<Metadata>> Metadata; // polymorphic type
		std::vector<Triangle> MeshBuffer;
		std::vector<uint32_t> IndexBuffer; // indirection between BVHAccel::Node and triangles in ResourcePool::meshBuffer
		std::vector<BVHAccel::Node> NodeBuffer;
		std::vector<unsigned char> TextureBuffer;

		template <typename T>
		std::shared_ptr<T> Get(const LR_GUID& guid) const {
			auto it = Metadata.find(guid);
			if (it == Metadata.end()) {
				return nullptr;
			}
			return std::dynamic_pointer_cast<T>(it->second);
		}
	};

	class Manager {
	public:
		Manager(ResourcePool* pool = nullptr)
			: m_ResourcePool(pool) {
		}
		~Manager() = default;
		
		inline void SetResourcePool(ResourcePool* pool) {
			m_ResourcePool = pool;
		}

		LR_GUID LoadAsset(const std::filesystem::path& path);

	private:
		ResourcePool* m_ResourcePool = nullptr;
		LR_GUID LoadMesh(const std::filesystem::path& path);
		// 0: original (default), 1: grayscale, 2: grayscale+alpha, 3: RGB, 4: RGBA
		LR_GUID LoadTexture(const std::filesystem::path& path, const int channels = 0);
	};
} 