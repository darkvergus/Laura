#pragma once

#include "lrpch.h"
#include <filesystem>
#include "Project/Scene/Scene.h"
#include "Project/ProjectUtilities.h"
#include "Core/GUID.h"

namespace Laura
{
	// ============================================================================
	// SCENE MANAGER
	// ----------------------------------------------------------------------------
	// High-level system responsible for creating, tracking, and persisting scenes.
	// Owns all loaded Scene objects and manages their lifetimes.
	// Handles serialization and deserialization of .lrscene files.
	// ============================================================================
	class SceneManager {
	public:
		SceneManager() = default;
		~SceneManager() = default;

		/// Creates a new scene with the given name.
		/// - Assigns a new GUID.
		/// - Adds it to the internal scene map.
		/// - Returns the new scene's GUID.
		LR_GUID CreateScene(const std::string& name = "Empty Scene");

		/// Removes a scene from the project by its GUID.
		/// - If the scene is currently active, clears that field.
		/// - Logs a warning if the scene was not found.
		void DeleteScene(LR_GUID guid);

		bool SetOpenScene(LR_GUID guid);
		std::shared_ptr<Scene> GetOpenScene() const;

		/// Saves all loaded scenes as `.lrscene` files into the specified folder.
		/// - Overwrites existing files with the same GUID.
		/// - Attempts to remove any orphaned scene files not backed by in-memory scenes.
		/// - Logs warnings on failures.
		/// - Does not throw; always tries to complete all work.
		void SaveScenesToFolder(const std::filesystem::path& folderpath) const;

		/// Loads all `.lrscene` files from the specified folder into memory.
		/// - Skips and logs any files that fail to deserialize.
		/// - Does not throw; continues loading remaining scenes.
		void LoadScenesFromFolder(const std::filesystem::path& folderpath);

		/// Iterators to enable range-based for loops over SceneManager:
		/// e.g. `for (auto& scene : SceneManager) { ... }`
		auto begin()       { return m_Scenes.begin(); }
		auto end()         { return m_Scenes.end(); }
		auto begin() const { return m_Scenes.begin(); }
		auto end()   const { return m_Scenes.end(); }

		/// Finds and returns a shared pointer to the Scene with the given GUID.
		/// Returns nullptr if the GUID is invalid or if no matching Scene is found.
		inline std::shared_ptr<Scene> find(LR_GUID guid) const {
			if (guid == LR_GUID::INVALID) {
				return nullptr;
			}
			if (auto it = m_Scenes.find(guid); it != m_Scenes.end()){
				return it->second;
			}
			return nullptr;
		}

	private:
		std::unordered_map<LR_GUID, std::shared_ptr<Scene>> m_Scenes;
		LR_GUID m_OpenSceneGuid = LR_GUID::INVALID;

		// Utilities ========================
		inline static LR_GUID ExtractGuidFromScenepath(const std::filesystem::path& scenepath) {
			std::string stem = scenepath.filename().stem().string(); // remove extension
			try {
				return LR_GUID(std::stoull(stem)); // convert stem to unsigned long long GUID
			}
			catch (const std::exception&) {
				LOG_ENGINE_WARN("ExtractGuid: filename \"{0}\" isn't a valid unsigned integer - skipping", scenepath.string());
				return LR_GUID::INVALID;
			}
		}

		inline static std::filesystem::path ComposeScenepathFromGuid(const std::filesystem::path& folderpath, LR_GUID guid) {
			return folderpath / (std::to_string(static_cast<uint64_t>(guid)) + SCENE_FILE_EXTENSION);
		}
	};
}
