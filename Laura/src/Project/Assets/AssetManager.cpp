#include "AssetManager.h"
#include "AssetManager.h"
#include "AssetManager.h"
#include "AssetManager.h"
#include "AssetManager.h"
#include "AssetManager.h"
#include "AssetManager.h"
#include "AssetManager.h"
#include "Project/Assets/AssetManager.h"
#include "Project/ProjectUtilities.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>
#include <yaml-cpp/yaml.h>
#include <optional>

namespace Laura
{

	// META FILE ------------------------------------------------------------------------------
    bool SaveMetafile(const std::filesystem::path& metafilePath, const AssetMetafile& assetMetafile) {
        LOG_ENGINE_INFO("Saving Metafile: {0}", metafilePath.string());

        YAML::Emitter out;
        out << YAML::BeginMap 
            << YAML::Key << "GUID" << YAML::Value << (uint64_t)assetMetafile.guid 
            << YAML::EndMap;

		std::ofstream fout(filepath);
		if (!fout.is_open()) {
			LOG_ENGINE_WARN("SaveMetaFile Failed to open file: {0}", metafilePath.string());
			return false;
		}
		fout << out.c_str();
		return true;
    }


    std::optional<AssetMetafile> LoadMetafile(const std::filesystem::path& metafilePath) {
        LOG_ENGINE_INFO("Loading Metafile: {0}", metafilePath.string());

        if (!std::filesystem::exists(metafilePath)) {
            LOG_ENGINE_WARN("Filepath does not exist: {0}", metafilePath.string());
            return std::nullopt;
        }

        YAML::Node root;
        try {
            root = YAML::LoadFile(metafilePath.string());
            AssetMetafile metafile;
            metafile.guid = (LR_GUID)root["GUID"].as<uint64_t>();
            return std::make_optional(metafile);
        }
        catch (const std::exception& e) {
            LOG_ENGINE_WARN("Failed to load metafile {}: {}", metafilePath.string(), e.what());
	        return std::nullopt;
        }
    }


     

    // ASSET MANAGER ---------------------------------------------------------------------------
    AssetManager::AssetManager()
        : m_AssetPool(std::make_shared<AssetPool>()) {
    }


    LR_GUID AssetManager::ImportAsset(const std::filesystem::path& assetpath) {
        if (!std::filesystem::exists(assetpath) || std::filesystem::is_directory(assetpath)) {
            LOG_ENGINE_WARN("Invalid assetpath: {0}", assetpath.string());
            return LR_GUID::INVALID;
        }

        LR_GUID guid;
        AssetMetafile metafile{guid};
        auto metaFilepath = AppendExtension(assetpath, ASSET_META_FILE_EXTENSION);
        if (!SaveMetafile(metaFilepath, metafile)) {
            return LR_GUID::INVALID;
        }

        if (!LoadAssetFile(assetpath, guid)) {
            std::filesystem::remove(metaFilepath); // cleanup metafile
            LOG_ENGINE_WARN("Failed to load asset after saving metafile. Removed metafile: {}", metaFilepath.string());
            return LR_GUID::INVALID;
        }
        return guid;
    }


	void AssetManager::SaveAssetPoolToFolder(const std::filesystem::path& folderpath) {
		// Delete all existing metafiles which don't have GUID within the asset pool
		for (const auto& metapath : FindFilesInFolder(folderpath, ASSET_META_FILE_EXTENSION)) {
			auto maybeMetafile = LoadMetafile(metapath);
			if (!maybeMetafile.has_value()) {
				LOG_ENGINE_WARN("Unable to check metafile GUID: {0}", metapath.string());
				continue;
			}
			LR_GUID guid = maybeMetafile->guid;
			if (m_AssetPool->Metadata.find(guid) == m_AssetPool->Metadata.end()) {
				std::filesystem::remove(metapath);
			}
		}

		// Save metafiles for all assets in the asset pool
		for (const auto& [guid, metadataPair] : m_AssetPool->Metadata) {
			const auto& [metadata, metadataExtension] = metadataPair;
			if (metadataExtension && IsFileInFolder(metadataExtension->sourcePath, folderpath)) {
				AssetMetafile metafile{ guid };
				auto metapath = AppendExtension(metadataExtension->sourcePath, ASSET_META_FILE_EXTENSION);
				if (!SaveMetafile(metapath, metafile)) {
					LOG_ENGINE_WARN("Unable to save metafile: {0}", metapath.string());
				}
			} else {
				LOG_ENGINE_WARN("Asset not found in the folder: {0}", 
							   metadataExtension ? metadataExtension->sourcePath.string() : "<no sourcePath>");
			}
		}
	}


	void AssetManager::LoadAssetPoolFromFolder(const std::filesystem::path& folderpath) {
		for (const auto& metapath : FindFilesInFolder(folderpath, ASSET_META_FILE_EXTENSION)) {
			// check if the asset file exists next to the .lrmeta file
			const auto assetpath = StripExtension(metapath);
			if (!std::filesystem::exists(assetpath)) {
				LOG_ENGINE_WARN("Missing asset file for metafile: {}", metapath.string());
				continue;
			}

			auto maybeMetafile = LoadMetafile(metapath);
			if (!maybeMetafile.has_value()) {
				LOG_ENGINE_WARN("Unable to load metafile: {}", metapath.string());
				continue;
			}

			if (!LoadAssetFile(assetpath, maybeMetafile->guid)) {
				LOG_ENGINE_WARN("Failed to load asset: {}", assetpath.string());
				continue;
			}
		}
	}


	bool AssetManager::LoadAssetFile(const std::filesystem::path& assetpath, LR_GUID guid) {
		if (!std::filesystem::exists(assetpath) || !std::filesystem::is_regular_file(assetpath) || !assetpath.has_extension()) {
			LOG_ENGINE_ERROR("Invalid asset path: {0}", assetpath.string());
			return false;
		}

		const std::string extension = assetpath.extension().string();
		for (const auto& SUPPORTED_FORMAT : SUPPORTED_MESH_FILE_FORMATS) {
			if (extension == SUPPORTED_FORMAT) {
				return LoadMesh(assetpath, guid);
			}
		}
		for (const auto& SUPPORTED_FORMAT : SUPPORTED_TEXTURE_FILE_FORMATS) {
			if (extension == SUPPORTED_FORMAT) {
				return LoadTexture(assetpath, 4, guid);
			}
		}
		LOG_ENGINE_WARN("Unknown file extension: {}", extension);
		return false;
	}



	bool AssetManager::LoadMesh(const std::filesystem::path& assetpath, LR_GUID guid) {
		auto timerStart = std::chrono::high_resolution_clock::now();

		if (!m_AssetPool) {
			LOG_ENGINE_CRITICAL("LoadMesh({0}) called without a valid AssetPool!", assetpath.string());
			return false;
		}

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(assetpath.string(), aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene) {
			LOG_ENGINE_CRITICAL("LoadMesh({0}, {1}) failed to load scene.", assetpath.string(), static_cast<uint64_t>(guid));
			return false;
		}

		size_t triCount = 0;
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
			triCount += scene->mMeshes[i]->mNumFaces;

		std::vector<Triangle>& meshBuffer = m_AssetPool->MeshBuffer;

		auto metadata = std::make_shared<MeshMetadata>();
		metadata->firstTriIdx = meshBuffer.size();
		metadata->TriCount = triCount;

		auto metadataExtension = std::make_shared<MeshMetadataExtension>();
		metadataExtension->sourcePath = assetpath;
		metadataExtension->fileSizeInBytes = std::filesystem::file_size(assetpath);

		meshBuffer.reserve(meshBuffer.size() + triCount);

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* subMesh = scene->mMeshes[i];
			const aiVector3D* verts = subMesh->mVertices;

			for (unsigned int j = 0; j < subMesh->mNumFaces; ++j) {
				const aiFace& face = subMesh->mFaces[j];
				if (face.mNumIndices != 3) continue;

				const auto& idx = face.mIndices;
				meshBuffer.emplace_back(Triangle({
					glm::vec4(verts[idx[0]].x, verts[idx[0]].y, verts[idx[0]].z, 0.0f),
					glm::vec4(verts[idx[1]].x, verts[idx[1]].y, verts[idx[1]].z, 0.0f),
					glm::vec4(verts[idx[2]].x, verts[idx[2]].y, verts[idx[2]].z, 0.0f)
				}));
			}
		}

		m_AssetPool->MarkUpdated(AssetPool::ResourceType::MeshBuffer);

		// Build BVH
		BVHAccel bvh(meshBuffer, metadata->firstTriIdx, metadata->TriCount);
		bvh.Build(m_AssetPool->NodeBuffer, m_AssetPool->IndexBuffer, metadata->firstNodeIdx, metadata->nodeCount);

		m_AssetPool->MarkUpdated(AssetPool::ResourceType::NodeBuffer);
		m_AssetPool->MarkUpdated(AssetPool::ResourceType::IndexBuffer);

		metadataExtension->loadTimeMs =
			std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - timerStart).count();

		m_AssetPool->Metadata[guid] = { metadata, metadataExtension };
		m_AssetPool->MarkUpdated(AssetPool::ResourceType::Metadata);

		LOG_ENGINE_INFO("LoadMesh({0}, {1}) loaded {2} triangles.", assetpath.string(), guid, triCount);
		return true;
	}



	bool AssetManager::LoadTexture(const std::filesystem::path& assetpath, int channels, LR_GUID guid) {
		auto timerStart = std::chrono::high_resolution_clock::now();

		if (!m_AssetPool) {
			LOG_ENGINE_CRITICAL("LoadTexture({0}) called without a valid AssetPool!", assetpath.string());
			return false;
		}

		int width, height, channelsInFile;
		stbi_set_flip_vertically_on_load(1); // OpenGL-style orientation
		unsigned char* data = stbi_load(assetpath.string().c_str(), &width, &height, &channelsInFile, channels);
		if (!data) {
			LOG_ENGINE_CRITICAL("LoadTexture({0}, channels={1}, guid={2}) failed to load.", assetpath.string(), channels, guid);
			return false;
		}

		const int actualChannels = (channels == 0) ? channelsInFile : channels;
		const size_t totalBytes = width * height * actualChannels;

		std::vector<unsigned char>& textureBuffer = m_AssetPool->TextureBuffer;
		textureBuffer.reserve(textureBuffer.size() + totalBytes);
		textureBuffer.insert(textureBuffer.end(), data, data + totalBytes);
		stbi_image_free(data);

		auto metadata = std::make_shared<TextureMetadata>();
		metadata->texStartIdx = textureBuffer.size() - totalBytes;
		metadata->width = width;
		metadata->height = height;
		metadata->channels = actualChannels;

		auto metadataExt = std::make_shared<TextureMetadataExtension>();
		metadataExt->sourcePath = assetpath;
		metadataExt->fileSizeInBytes = std::filesystem::file_size(assetpath);
		metadataExt->loadTimeMs =
			std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - timerStart).count();

		m_AssetPool->Metadata[guid] = { metadata, metadataExt };
		m_AssetPool->MarkUpdated(AssetPool::ResourceType::TextureBuffer);
		m_AssetPool->MarkUpdated(AssetPool::ResourceType::Metadata);

		LOG_ENGINE_INFO("LoadTexture({0}, {1}) loaded {2}x{3} texture ({4} channels).",
						assetpath.string(), guid, width, height, actualChannels);
		return true;
	}
}