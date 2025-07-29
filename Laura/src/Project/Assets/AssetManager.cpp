#include "AssetManager.h"
#include "Project/Assets/AssetManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>

namespace Laura
{

	LR_GUID AssetManager::LoadMesh(const std::filesystem::path& path) {
		auto timerStart = std::chrono::high_resolution_clock::now();

        if (!m_ResourcePool) {
            LOG_ENGINE_CRITICAL("Asset::Manager::LoadMesh({0}) called before a valid ResourcePool has been assigned!", path.string());
            return LR_GUID::INVALID;
        }

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.string(), aiProcessPreset_TargetRealtime_MaxQuality);
        if (!scene) {
            LOG_ENGINE_CRITICAL("Asset::Manager::LoadMesh({0}) failed to load!", path.string());
            return LR_GUID::INVALID;
        }

        // count the number of triangles in file
        size_t numTris = 0;
        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            numTris += scene->mMeshes[i]->mNumFaces;
        }
        
        std::vector<Triangle>& meshBuffer = m_ResourcePool->MeshBuffer;
        // populate some metadata
        auto metadata = std::make_shared<MeshMetadata>();
        metadata->firstTriIdx = meshBuffer.size();
        metadata->TriCount = numTris;
        auto metadataExtension = std::make_shared<MeshMetadataExtension>();
        metadataExtension->sourcePath = path;
        metadataExtension->fileSizeInBytes = std::filesystem::file_size(path);
        
        meshBuffer.reserve(meshBuffer.size() + metadata->TriCount);

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            const aiMesh* subMesh = scene->mMeshes[i];
            const auto verts = subMesh->mVertices;
            for (unsigned int j = 0; j < subMesh->mNumFaces; ++j) {
                const auto& face = subMesh->mFaces[j];

                if (face.mNumIndices != 3) {
                    continue;
                }

                auto idxs = face.mIndices;
                meshBuffer.emplace_back( Triangle({ 
                    glm::vec4(verts[idxs[0]].x, verts[idxs[0]].y, verts[idxs[0]].z, 0.0f),
                    glm::vec4(verts[idxs[1]].x, verts[idxs[1]].y, verts[idxs[1]].z, 0.0f),
                    glm::vec4(verts[idxs[2]].x, verts[idxs[2]].y, verts[idxs[2]].z, 0.0f)
                    })
                );
            }
        }
        m_ResourcePool->MarkUpdated(ResourcePool::ResourceType::MeshBuffer);

        LOG_ENGINE_INFO("Asset::Manager::LoadMesh({0}) loaded {1} triangles.", path.string(), numTris);

        BVHAccel BVH(meshBuffer, metadata->firstTriIdx, metadata->TriCount); // pass in the mesh
        BVH.Build(m_ResourcePool->NodeBuffer, m_ResourcePool->IndexBuffer, metadata->firstNodeIdx, metadata->nodeCount); // populate in place
        m_ResourcePool->MarkUpdated(ResourcePool::ResourceType::NodeBuffer);
        m_ResourcePool->MarkUpdated(ResourcePool::ResourceType::IndexBuffer);

        LR_GUID guid;

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		metadataExtension->loadTimeMs = std::chrono::duration<double, std::milli>(end - timerStart).count();
        
        m_ResourcePool->Metadata[guid] = { metadata, metadataExtension };
        m_ResourcePool->MarkUpdated(ResourcePool::ResourceType::Metadata);
        return guid;
    }

	LR_GUID AssetManager::LoadTexture(const std::filesystem::path& path, const int desiredChannels) {
		auto timerStart = std::chrono::high_resolution_clock::now();
        if (!m_ResourcePool){
            LOG_ENGINE_CRITICAL("Asset::Manager::LoadTexture({0}, {1}) called before a valid ResourcePool has been assigned!", path.string(), desiredChannels);
            return LR_GUID::INVALID;
        }

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1); // ensure (0,0) is bottom-left
		unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, desiredChannels);
        if (!data) {
			LOG_ENGINE_CRITICAL("Asset::Manager::LoadTexture({0}, {1}) failed to load!", path.string(), desiredChannels);
			return LR_GUID::INVALID;
		}
        
        std::vector<unsigned char>& textureBuffer = m_ResourcePool->TextureBuffer;

        auto metadata = std::make_shared<TextureMetadata>();
        metadata->texStartIdx = textureBuffer.size();
        metadata->width = width;
        metadata->height = height;
        metadata->channels = (desiredChannels == 0) ? channels : desiredChannels;

        auto metadataExtension = std::make_shared<TextureMetadataExtension>();
        metadataExtension->sourcePath = path;
        metadataExtension->fileSizeInBytes = std::filesystem::file_size(path);

        const size_t totalBytes = metadata->width * metadata->height * metadata->channels;
        textureBuffer.reserve(textureBuffer.size() + totalBytes);
        textureBuffer.insert(textureBuffer.end(), data, data + totalBytes);
        m_ResourcePool->MarkUpdated(ResourcePool::ResourceType::TextureBuffer);
        stbi_image_free(data);
	    	
        LR_GUID guid;

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		metadataExtension->loadTimeMs = std::chrono::duration<double, std::milli>(end - timerStart).count();
        
        m_ResourcePool->Metadata[guid] = { metadata, metadataExtension };
        m_ResourcePool->MarkUpdated(ResourcePool::ResourceType::Metadata);
		return guid;
	}


    LR_GUID AssetManager::LoadAsset(const std::filesystem::path& path) {
        if (!path.has_extension()) {
            LOG_ENGINE_ERROR("LoadAsset: File has no extension: {}", path.string());
            return LR_GUID::INVALID;
        }

        std::string ext = path.extension().string();
        for ( const auto& SUPPORTED_EXT : SUPPORTED_MESH_FILE_FORMATS) {
            if (ext == SUPPORTED_EXT) {
                return LoadMesh(path);
            }
        }
        for (auto SUPPORTED_EXT : SUPPORTED_TEXTURE_FILE_FORMATS) {
            if (ext == SUPPORTED_EXT) {
                return LoadTexture(path, 4);
            }
        }

        LOG_ENGINE_WARN("LoadAsset: Unknown file extension: {}", ext);
        return LR_GUID::INVALID;
    }
}