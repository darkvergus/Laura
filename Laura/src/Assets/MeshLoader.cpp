#include "Assets/MeshLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "core/Log.h"

namespace Laura
{
    namespace MeshLoader
    {
        std::shared_ptr<std::vector<Triangle>> loadMesh(const std::string& filepath)
        {
            bool hasNonTriangleFaces = false;
            std::vector<Triangle> triangleMesh;

            // Import the scene using Assimp
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(filepath, aiProcessPreset_TargetRealtime_MaxQuality);

            if (!scene) {
                LR_CORE_CRITICAL("Failed to load mesh: {}", filepath);
                return nullptr; // Return empty mesh
            }

            for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
                const aiMesh* currentMesh = scene->mMeshes[i];

                for (unsigned int j = 0; j < currentMesh->mNumFaces; ++j) {
                    const aiFace& face = currentMesh->mFaces[j];

                    // Ensure the face is a triangle
                    if (face.mNumIndices != 3) {
                        hasNonTriangleFaces = true;
                        continue; // Skip non-triangle faces
                    }

                    Triangle triangle;
                    triangle.v1 = glm::vec3(currentMesh->mVertices[face.mIndices[0]].x,
                        currentMesh->mVertices[face.mIndices[0]].y,
                        currentMesh->mVertices[face.mIndices[0]].z);

                    triangle.v2 = glm::vec3(currentMesh->mVertices[face.mIndices[1]].x,
                        currentMesh->mVertices[face.mIndices[1]].y,
                        currentMesh->mVertices[face.mIndices[1]].z);

                    triangle.v3 = glm::vec3(currentMesh->mVertices[face.mIndices[2]].x,
                        currentMesh->mVertices[face.mIndices[2]].y,
                        currentMesh->mVertices[face.mIndices[2]].z);

                    if (currentMesh->HasNormals()) {
                        triangle.NA = glm::vec3(currentMesh->mNormals[face.mIndices[0]].x,
                            currentMesh->mNormals[face.mIndices[0]].y,
                            currentMesh->mNormals[face.mIndices[0]].z);

                        triangle.NB = glm::vec3(currentMesh->mNormals[face.mIndices[1]].x,
                            currentMesh->mNormals[face.mIndices[1]].y,
                            currentMesh->mNormals[face.mIndices[1]].z);

                        triangle.NC = glm::vec3(currentMesh->mNormals[face.mIndices[2]].x,
                            currentMesh->mNormals[face.mIndices[2]].y,
                            currentMesh->mNormals[face.mIndices[2]].z);
                    }
                    else {
                        // Set default normals if none are provided
                        triangle.NA = triangle.NB = triangle.NC = glm::vec3(0.0f, 1.0f, 0.0f);
                        LR_CORE_WARN("Mesh has no normals, using default normals.");
                    }

                    // Calculate centroid
                    triangle.centroid = (triangle.v1 + triangle.v2 + triangle.v3) / 3.0f;

                    // Default material properties
                    triangle.material.color = glm::vec3(0.7f, 0.7f, 0.7f); // Light grey
                    triangle.material.emissionColor = glm::vec3(0.0f, 0.0f, 0.0f);
                    triangle.material.emissionStrength = 0.0f;

                    triangleMesh.push_back(triangle);
                }
            }

            if (hasNonTriangleFaces)
            {
                LR_CORE_WARN("Skipping non-triangle face in mesh: {}", filepath);
            }

            LR_CORE_INFO("Loaded {} triangles from mesh: {}", triangleMesh.size(), filepath);

            return std::make_shared<std::vector<Triangle>>(triangleMesh);
        }
    }
}