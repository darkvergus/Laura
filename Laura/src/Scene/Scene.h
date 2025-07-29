#pragma once

#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "entt/entt.hpp"
#include "Scene/Entity.h"
#include "Scene/Components.h"

#define SCENE_FILE_EXTENSION ".lrscn"

namespace Laura
{

	class Scene {
	public:
		Scene();
		~Scene();
		EntityHandle CreateEntity();
		void DestroyEntity(EntityHandle entity);

		void OnStart();
		void OnUpdate();
		void OnShutdown();

		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);

		inline entt::registry* GetRegistry() const { return m_Registry; }

		inline LR_GUID GetSkyboxGUID() const { return m_SkyboxGUID; }
		inline void SetSkyboxGUID(LR_GUID guid) { m_SkyboxGUID = guid; }

	private:
		LR_GUID m_SkyboxGUID;
		entt::registry* m_Registry;
	};
}