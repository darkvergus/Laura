#include "Assets.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image/stb_image.h>

namespace Laura::Asset
{

	LR_GUID Manager::LoadMesh(const std::filesystem::path& path)
    {
        std::vector<Vertex>& mesh = resourcePool->meshVerts;
        auto metadata = std::make_shared<MeshMetadata>();
        metadata->vertStartIdx = mesh.size();
        metadata->path = path;

        const std::string pathStr = path.string();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(pathStr, aiProcessPreset_TargetRealtime_MaxQuality);

        if (!scene) {
            LR_CORE_CRITICAL("Failed to load mesh: {}", pathStr);
            return LR_GUID(0);
        }

        size_t numTris = 0;
        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            numTris += scene->mMeshes[i]->mNumFaces;
        }
        
        mesh.reserve(mesh.size() + 3*numTris);

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            const aiMesh* subMesh = scene->mMeshes[i];
            const auto verts = subMesh->mVertices;
            for (unsigned int j = 0; j < subMesh->mNumFaces; ++j) {
                const auto& face = subMesh->mFaces[j];

                // ignore index buffer store triangles directly
                auto idxs = face.mIndices;               
                mesh.push_back(glm::vec3(verts[idxs[0]].x, verts[idxs[0]].y, verts[idxs[0]].z));
                mesh.push_back(glm::vec3(verts[idxs[1]].x, verts[idxs[1]].y, verts[idxs[1]].z));
                mesh.push_back(glm::vec3(verts[idxs[2]].x, verts[idxs[2]].y, verts[idxs[2]].z));
            }
        }

        LR_CORE_INFO("Loaded {} triangles from mesh: {}", mesh.size(), pathStr);

        // TODO BVH BUILDING here

        LR_GUID guid;
        resourcePool->metadata[guid] = metadata;
        return guid;
    }

	LR_GUID Manager::LoadTexture(const std::filesystem::path& path, const int desiredChannels)
	{
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1); // ensure (0,0) is bottom-left
		unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, desiredChannels);
        if (!data) {
			LR_CORE_CRITICAL("AssetLoader::LoadTexture() failed to load: {0}", path.string());
			return LR_GUID(0);
		}
        
        std::vector<unsigned char>& textures = resourcePool->textureData;

        auto metadata = std::make_shared<TextureMetadata>();
        metadata->texStartIdx = textures.size();
        metadata->width = width;
        metadata->height = height;
        metadata->channels = (desiredChannels == 0) ? channels : desiredChannels;
        metadata->path = path;

        const size_t totalBytes = metadata->width * metadata->height * metadata->channels;
        textures.reserve(textures.size() + totalBytes);
        textures.insert(textures.end(), data, data + totalBytes);
        stbi_image_free(data);
		
        LR_GUID guid;
        resourcePool->metadata[guid] = metadata;
		return guid;
	}
}