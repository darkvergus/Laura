#pragma once

#include "lrpch.h"

#include "Core/GUID.h"

#include <filesystem>

namespace Laura::Asset {

	// According to std430 - 48 bytes
	// For simplicity - storing one extra padding float per vertex ( not ideal but might use the space for some other data in the future)
	struct Triangle {
		glm::vec4 v0 = {}, v1 = {}, v2 = {};
	};
	struct Metadata {
        virtual ~Metadata() = default;
        std::filesystem::path path;
    };

    struct MeshMetadata : public Metadata {
        uint32_t firstTriIdx  = 0;
        uint32_t TriCount     = 0;
        uint32_t firstNodeIdx = 0;
        uint32_t nodeCount    = 0;
        ~MeshMetadata() override = default;
    };

    struct TextureMetadata : public Metadata {
        uint32_t texStartIdx = 0;
        int32_t  width       = 0;
        int32_t  height      = 0;
        int32_t  channels    = 0;
        ~TextureMetadata() override = default;
    };


}