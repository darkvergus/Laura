#pragma once

#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "entt/entt.hpp"
#include "Project/Scene/Entity.h"
#include "Project/Scene/Components.h"

namespace Laura
{

	// ============================================================================
	// SCENE FILE (.lrscene)
	// ----------------------------------------------------------------------------
	// Represents a scene containing entities, components, and metadata.
	// Serialization and deserialization handled by free functions.
	// ============================================================================
	
	#define SCENE_FILE_EXTENSION ".lrscn"

	class Scene {
	public:
		explicit Scene(std::string name)
			: m_SceneName(std::move(name)) {
			m_Registry = new entt::registry();
		}

		~Scene() {
			delete m_Registry;
		}

		// non movable, non copyable
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		EntityHandle CreateEntity();
		void DestroyEntity(EntityHandle entity);

		void OnStart();
		void OnUpdate();
		void OnShutdown();

		inline entt::registry*		GetRegistry()	const { return m_Registry;	 }
		inline LR_GUID				GetGuid()		const { return m_SceneGUID;  }
		inline LR_GUID				GetSkyboxGuid() const { return m_SkyboxGUID; }
		inline const std::string&	GetName()		const { return m_SceneName;  }

		inline std::string&			GetMutableName()	{ return m_SceneName; }

		inline void			SetSkyboxGuid(LR_GUID guid) { m_SkyboxGUID = guid;			  }
		inline void			SetName(std::string name)	{ m_SceneName  = std::move(name); }

	private:
		LR_GUID		m_SceneGUID;
		LR_GUID		m_SkyboxGUID;
		std::string m_SceneName;
		entt::registry* m_Registry;

		friend bool SaveSceneFile(const std::filesystem::path& scenepath, std::shared_ptr<const Scene> scene);
		friend std::shared_ptr<Scene> LoadSceneFile(const std::filesystem::path& scenepath);
	};


	// ============================================================================
	// SERIALIZATION / DESERIALIZATION
	// ============================================================================

	/// Saves scene data to 'scenepath' (full path with filename and extension, e.g. "c:/dev/scene.lrscn").
	/// The file will be created or overwritten; it does not need to exist beforehand.
	/// The parent directory must exist; this function does not create directories.
	/// Returns true on success, false otherwise.
	bool SaveSceneFile(const std::filesystem::path& scenepath, std::shared_ptr<const Scene> scene);

	/// Loads a scene from the scene file at 'scenepath'.
	/// 'sceneFilePath' must be the full path including filename and extension.
	/// Returns a shared_ptr to the Scene or nullptr on failure.
	std::shared_ptr<Scene> LoadSceneFile(const std::filesystem::path& scenepath);
}