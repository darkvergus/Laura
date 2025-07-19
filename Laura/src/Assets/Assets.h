#pragma once

#include "lrpch.h"

#include "Assets/AssetTypes.h"
#include "Assets/BVHAccel.h"

#include "Core/GUID.h"

#include <array>
#include <filesystem>

constexpr const char* SUPPORTED_MESH_FILE_FORMATS[]		= { ".fbx", ".obj" ,".gltf", ".glb" };
constexpr const char* SUPPORTED_TEXTURE_FILE_FORMATS[]	= { ".png", ".jpg", ".jpeg", ".tga", ".bpm", ".hdr" };

namespace Laura::Asset
{
	enum struct ResourceType {
		Metadata,
		MeshBuffer,
		IndexBuffer,
		NodeBuffer,
		TextureBuffer,
		COUNT
	};

	using MetadataPair = std::pair<
		std::shared_ptr<Metadata>, 
		std::shared_ptr<MetadataExtension>
	>;
	struct ResourcePool { 
		std::unordered_map<LR_GUID, MetadataPair> Metadata; // polymorphic type
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
			const auto& [metadata, metadataExtension] = it->second;
			return std::dynamic_pointer_cast<T>(metadata);
		}

		/*
			Clever versioning update flag system that works with multiple listeners(currently just the renderer)
			listener sets up a static lastUpdateId and if lastUpdateId != rp.GetUpdateId() and update happened
		*/
		inline void MarkUpdated(ResourceType type) {
			m_UpdateVersions[static_cast<size_t>(type)]++;
		}

		inline uint32_t GetUpdateVersion(ResourceType type) const {
			return m_UpdateVersions[static_cast<size_t>(type)];
		}
	private:
		std::array<uint32_t, static_cast<size_t>(ResourceType::COUNT)> m_UpdateVersions = {}; // initialize with 0s
	};

	//////////////////////////////////////////

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