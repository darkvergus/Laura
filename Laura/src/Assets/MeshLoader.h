#pragma once

#include "lrpch.h"
#include "Geometry/Triangle.h"

namespace Laura 
{
    namespace MeshLoader
    {
        // Loads a 3D mesh from various filetypes .obj .glb .gltf ...
        std::vector<Triangle> loadMesh(const std::string& filepath);
    }
}