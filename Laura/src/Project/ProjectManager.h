#pragma once 

#include "lrpch.h"
#include <filesystem>
#include "Renderer/RenderSettings.h"
#include "Project/Scene/SceneManager.h"
#include "Project/Assets/AssetManager.h"

namespace Laura 
{

	// ============================================================================ 
	// PROJECT FILE (.lrproj)
	// ----------------------------------------------------------------------------
	// Provides serialization and deserialization of project settings.
	// Used internally by the ProjectManager to persist global project data.
	// ============================================================================
	#define PROJECT_FILE_EXTENSION ".lrproj"

	struct ProjectFile {
		LR_GUID bootSceneGuid = LR_GUID::INVALID;
		RenderSettings runtimeRenderSettings{};

		ProjectFile(LR_GUID bootSceneGuid = LR_GUID::INVALID) : bootSceneGuid(bootSceneGuid) {}
	};
	
	/// Serialize the 'projectFile' as-is at the location 'projectFilepath'.
	/// Returns true on success.
	bool SaveProjectFile(const std::filesystem::path& projectFilepath, const ProjectFile& projectFile);

	/// Deserialize from 'projectFilepath' and return 'ProjectFile'.
	/// Returns std::nullopt if unsuccessful.
	std::optional<ProjectFile> LoadProjectFile(const std::filesystem::path& projectFilepath);

	/// Computes the absolute path to the project file (.lrproj) given the project folder.
	/// Example: Input folder `/MyProject/` -> Output `/MyProject/MyProject.lrproj`
	inline std::filesystem::path ComposeProjectFilepath(const std::filesystem::path& folderpath) {
		std::string folderName = folderpath.filename().string();
		return folderpath / (folderName + PROJECT_FILE_EXTENSION);
	}




	// ============================================================================
	// PROJECT MANAGER
	// ----------------------------------------------------------------------------
	// High-level system responsible for creating, opening, saving, and managing
	// the lifetime of a project.
	// Owns the AssetManager and SceneManager used throughout the editor session.
	// ============================================================================
	class ProjectManager {
	public:
		ProjectManager() = default;
		~ProjectManager() = default;

		/// Creates a new project in the given folder.
		/// Initializes the AssetManager and SceneManager.
		/// Saves an initial project file.
		/// Returns true on success.
		bool NewProject(const std::filesystem::path& folderpath);

		/// Opens an existing project from the given folder.
		/// Loads the project file and initializes managers.
		/// Returns true on success.
		bool OpenProject(const std::filesystem::path& projectfilePath);

		/// Saves the current project file (.lrproj) into the project folder.
		/// Returns true on success.
		bool SaveProject();

		/// Shuts down managers and clears project data without saving.
		void CloseProject();

		inline bool ProjectIsOpen() const { return !m_ProjectPath.empty(); }
		inline std::shared_ptr<SceneManager> GetSceneManager() const { return m_SceneManager; }
		inline std::shared_ptr<AssetManager> GetAssetManager() const { return m_AssetManager; }

		inline void SetBootSceneGuid(LR_GUID guid) { m_ProjectFile.bootSceneGuid = guid; }
		inline LR_GUID GetBootSceneGuid() const { return m_ProjectFile.bootSceneGuid; }
		inline bool IsBootScene(LR_GUID guid) { return m_ProjectFile.bootSceneGuid == guid; }

		inline RenderSettings& GetMutableRuntimeRenderSettings() { return m_ProjectFile.runtimeRenderSettings; }
	private:
		/// Filesystem path to the current project folder (where .lrproj lives).
		std::filesystem::path m_ProjectPath;
		ProjectFile m_ProjectFile;

		std::shared_ptr<SceneManager> m_SceneManager = nullptr;
		std::shared_ptr<AssetManager> m_AssetManager = nullptr;
	};
}