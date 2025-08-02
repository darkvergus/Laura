#include "Project/ProjectManager.h"

namespace Laura
{
	
	// PROJECT FILE ------------------------------------------------------------------------------
	bool SaveProjectFile(const std::filesystem::path& projectFilepath, const ProjectFile& projectFile) {
		if (!(projectFilepath.has_extension() && projectFilepath.extension() == PROJECT_FILE_EXTENSION)) {
			LOG_ENGINE_WARN("SaveProjectFile: invalid file extension '{}'.", projectFilepath.string());
			return false;
		}

		if (!std::filesystem::exists(projectFilepath.parent_path())) {
			LOG_ENGINE_WARN("SaveProjectFile: parent directory '{}' does not exist.", projectFilepath.parent_path().string());
			return false;
		}

		YAML::Emitter out;
		out << YAML::BeginMap
		<< YAML::Key << "bootSceneGuid" << YAML::Value << static_cast<uint64_t>(projectFile.bootSceneGuid)
		<< YAML::EndMap;

		std::ofstream fout(projectFilepath);
		if (!fout.is_open()) {
			LOG_ENGINE_ERROR("SaveProjectFile: could not open {0} for writing — permissions or path invalid", projectFilepath.string());
			return false;
		}
		fout << out.c_str();
		LOG_ENGINE_INFO("SaveProjectFile: wrote project data into {0}", projectFilepath.string());
		return true;
	}


	std::optional<ProjectFile> LoadProjectFile(const std::filesystem::path& projectFilepath) {
		if (!(std::filesystem::exists(projectFilepath) && std::filesystem::is_regular_file(projectFilepath) && 
			projectFilepath.has_extension() && projectFilepath.extension() == PROJECT_FILE_EXTENSION))
		{
			LOG_ENGINE_WARN("LoadProjectFile: invalid or missing project file: {0}", projectFilepath.string());
			return std::nullopt;
		}

		YAML::Node root;
		try {
			root = YAML::LoadFile(filepath.string());
			ProjectFile projectFile;
			projectFile.bootSceneGuid = static_cast<LR_GUID>(root["bootSceneGuid"].as<uint64_t>());
			LOG_ENGINE_INFO("LoadProjectFile: successfully loaded project file from {0}", projectFilepath.string());
			return std::make_optional(projectFile);
		}
		catch (const YAML::ParserException& e) {
			LOG_ENGINE_ERROR("LoadProjectFile: YAML parse error while reading {0}: {1}", projectFilepath.string(), e.what());
			return std::nullopt;
		}
	}




	// PROJECT MANAGER ----------------------------------------------------------------------------
	void ProjectManager::NewProject(const std::filesystem::path& folderpath) {
		if (std::filesystem::exists(folderpath)) {
			LOG_ENGINE_WARN("NewProject: folder already exists at {0}", folderpath.string());
			return false;
		}

		if (!std::filesystem::create_directory(folderpath)) {
			LOG_ENGINE_ERROR("NewProject: failed to create directory at {0}", folderpath.string());
			return false;
		}

		m_ProjectPath = folderpath;
		m_ProjectFile = ProjectFile{};

		m_AssetManager = std::make_shared<AssetManager>();
		m_SceneManager = std::make_shared<SceneManager>();

		if (!SaveProject()) {
			LOG_ENGINE_ERROR("NewProject: failed to save initial project file at {0}", folderpath.string());
			return false;
		}

		LOG_ENGINE_INFO("NewProject: successfully created project at {0}", folderpath.string());
		return true;
	}


	bool ProjectManager::OpenProject(const std::filesystem::path& folderpath) {
		if (!(std::filesystem::exists(folderpath) && std::filesystem::is_directory(folderpath))) {
			LOG_ENGINE_ERROR("OpenProject: project folder is invalid or does not exist at {0}", folderpath.string());
			return false;
		}

		m_ProjectPath = folderpath;

		std::filesystem::path projectFilepath = ComposeProjectFilepath(folderpath);
		if (!LoadProjectFile(projectFilepath)) {
			m_ProjectFile = ProjectFile{};
			LOG_ENGINE_WARN("OpenProject: failed to deserialize project file at {0}", projectFilepath.string());
		}

		m_AssetManager = std::make_shared<AssetManager>();
		m_SceneManager = std::make_shared<SceneManager>();

		if (!m_AssetManager->LoadAssetPoolFromFolder(folderpath)) {
			LOG_ENGINE_WARN("OpenProject: failed to load asset pool from {0}", folderpath.string());
		}
		if (!m_SceneManager->LoadScenesFromFolder(folderpath)) {
			LOG_ENGINE_WARN("OpenProject: failed to load scenes from {0}", folderpath.string());
		}

		LOG_ENGINE_INFO("OpenProject: successfully opened project at {0}", folderpath.string());
		return true;
	}


	bool ProjectManager::SaveProject(const std::filesystem::path& folderpath) {
		if (!(std::filesystem::exists(folderpath) && std::filesystem::is_directory(folderpath))) {
			LOG_ENGINE_ERROR("SaveProject: project folder is invalid or does not exist at {0}", folderpath.string());
			return false;
		}

		if (!IsProjectOpen()) {
			LOG_ENGINE_WARN("SaveProject: no project is currently open");
			return false;
		}

		bool success = true;
		std::filesystem::path projectFilepath = ComposeProjectFilepath(folderpath);

		if (!SaveProjectFile(projectFilepath, m_ProjectFile)) {
			LOG_ENGINE_ERROR("SaveProject: failed to serialize project file at {0}", projectFilepath.string());
			success = false;
		} else { LOG_ENGINE_INFO("SaveProject: wrote project data into {0}", projectFilepath.string()); }

		if (!m_SceneManager->SaveScenesToFolder(folderpath)) {
			LOG_ENGINE_ERROR("SaveProject: failed to save scenes to {0}", folderpath.string());
			success = false;
		} else { LOG_ENGINE_INFO("SaveProject: saved scenes to {0}", folderpath.string()); }

		if (!m_AssetManager->SaveAssetPoolToFolder(folderpath)) {
			LOG_ENGINE_ERROR("SaveProject: failed to save asset pool to {0}", folderpath.string());
			success = false;
		} else { LOG_ENGINE_INFO("SaveProject: saved asset pool to {0}", folderpath.string()); }

		return success;
	}

	void ProjectManager::CloseProject() {
		LOG_ENGINE_INFO("CloseProject: closing project at {0}", m_ProjectPath.string());

		m_ProjectPath.clear();
		m_ProjectFile = ProjectFile{};
		m_AssetManager = nullptr;
		m_SceneManager = nullptr;

		LOG_ENGINE_INFO("CloseProject: project closed successfully");
	}
}