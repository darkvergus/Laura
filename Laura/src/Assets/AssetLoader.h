#pragma once

#include "lrpch.h"
#include "Core/GUID.h"

#include <filesystem>

namespace Laura
{
	class Asset{
	public:
		virtual ~Asset() = default; // "virtual" Ensures proper cleanup of derived assets (e.g. Texture) 
	};

	struct Tri {
		glm::vec3 v0 = {}, v1 = {}, v2 = {};
	};

	class Mesh : public Asset{
	public:
		Mesh() = default;
		~Mesh() = default;
		inline const std::vector<Tri>& data() const { return m_Data; }
		inline std::vector<Tri>& data() { return m_Data; }
	private:
		std::vector<Tri> m_Data;
	};

	class Texture : public Asset {
	public:
		Texture() = default;
		~Texture() override;
		unsigned char* data = nullptr;
		int width = 0, height = 0, channels = 0;
	};


	class AssetLoader
	{
	public:
		AssetLoader() = default;
		~AssetLoader() = default;

		LR_GUID LoadMesh(const std::filesystem::path& path);
		/// channels: 0 = original (default), 1 = grayscale, 2 = grayscale+alpha, 3 = RGB, 4 = RGBA.
		LR_GUID LoadTexture(const std::filesystem::path& path, const int channels = 0);


		inline void Unload(const LR_GUID& guid){
			m_Assets.erase(guid);
		}

		template <typename T>
		std::shared_ptr<T> Get(const LR_GUID& guid) {
			auto it = m_Assets.find(guid);
			if (it == m_Assets.end()) 
				return nullptr;
			return std::dynamic_pointer_cast<T>(it->second);
		}

	private:
		std::unordered_map<LR_GUID, std::shared_ptr<Asset>> m_Assets;
	};
}