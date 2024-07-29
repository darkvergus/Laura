#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include "lrpch.h"
#include "Components/MeshComponent.h"

namespace Laura 
{
    // Loads a 3D mesh from various filetypes .obj .glb .gltf ...
    MeshComponent loadMesh(const std::string& filepath);
}

#endif // MESH_LOADER_H