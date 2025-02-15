#include "AssetLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image/stb_image.h>

namespace Laura
{
    Texture::~Texture() /* override */ {
        if (data) stbi_image_free(data);
    }

	LR_GUID AssetLoader::LoadMesh(const std::filesystem::path& path)
    {
        Mesh mesh;
        auto& meshData = mesh.data();

        const std::string pathStr = path.string();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(pathStr, aiProcessPreset_TargetRealtime_MaxQuality);

        if (!scene) {
            LR_CORE_CRITICAL("Failed to load mesh: {}", pathStr);
            return LR_GUID(0);
        }

        size_t capacity = 0;
        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            capacity += scene->mMeshes[i]->mNumFaces;
        }
        
        meshData.reserve(capacity);

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            const aiMesh* subMesh = scene->mMeshes[i];
            auto verts = subMesh->mVertices;
            for (unsigned int j = 0; j < subMesh->mNumFaces; ++j) {
                const aiFace& face = subMesh->mFaces[j];

                if (face.mNumIndices != 3) continue;

                // ignore index buffer store triangles directly
                Tri tri;
                auto idxs = face.mIndices;
                tri.v0 = glm::vec3(verts[idxs[0]].x, verts[idxs[0]].y, verts[idxs[0]].z);
                tri.v1 = glm::vec3(verts[idxs[1]].x, verts[idxs[1]].y, verts[idxs[1]].z);
                tri.v2 = glm::vec3(verts[idxs[2]].x, verts[idxs[2]].y, verts[idxs[2]].z);
                
                meshData.push_back(tri);
            }
        }

        LR_CORE_INFO("Loaded {} triangles from mesh: {}", meshData.size(), pathStr);

        LR_GUID guid;
        while (m_Assets.find(guid) != m_Assets.end()) {
            guid = LR_GUID();
        }
        m_Assets[guid] = std::make_shared<Mesh>(std::move(mesh));
        return guid;
    }

	LR_GUID AssetLoader::LoadTexture(const std::filesystem::path& path, const int channels)
	{
		Texture texture;

        const std::string pathStr = path.string();
		
        stbi_set_flip_vertically_on_load(1); // Flip image so (0,0) is bottom-left
		texture.data = stbi_load(pathStr.c_str(), &texture.width, &texture.height, &texture.channels, channels);
		
        if (!texture.data) {
			LR_CORE_CRITICAL("AssetLoader::LoadTexture() failed to load: {0}", pathStr);
			return LR_GUID(0);
		}

        LR_GUID guid;
        while (m_Assets.find(guid) != m_Assets.end()) {
            guid = LR_GUID();
        }
		m_Assets[guid] = std::make_shared<Texture>(std::move(texture));
		return guid;
	}
}