#include "Project/ProjectManager.h"

namespace Laura
{

	bool ProjectManager::NewProject(std::filesystem::path projectFolderpath) {
		if (std::filesystem::exists(projectFolderpath)) {
			LOG_ENGINE_WARN("Project already exists {0}", projectFolderpath);
			return false;
		}
		if (!std::filesystem::create_directory(projectFolderpath)) {
			LOG_ENGINE_WARN("Unable to create directory {0}", projectFolderpath);
			return false;
		}
		m_ProjectFolderpath = projectFolderpath;
		m_BootScene = LR_GUID::INVALID; // no boot scene yet configured
		m_AssetManager = std::make_shared<AssetManager>();
		m_SceneManager = std::make_shared<SceneManager>();
		return SaveProject();
	}

	bool ProjectManager::OpenProject(std::filesystem::path projectFolderpath) {
		if (!std::filesystem::exists(projectFolderpath)) {
			LOG_ENGINE_WARN("Project folder does not exist: {0}", projectFolderpath);
			return false;
		}

		if (!DeserializeProjFile(projectFolderpath)) {
			LOG_ENGINE_WARN("Unable to deserialize project file");
		}
		if (!m_SceneManager->DeserializeScenes(projectFolderpath)) {
			LOG_ENGINE_WARN("Unable to load all Scenes");
		}
		if (m_AssetManager->DeserializeAssetPool(projectFolderpath)) {
			LOG_ENGINE_WARN("Unable to Load resources");
		}
		m_ProjectFolderpath = projectFolderpath;
		return true;
	}

	bool ProjectManager::SaveProject() {
		if (!isProjectOpen()) {
			return false;
		}
		if (!SerializeProjFile(m_ProjectFolderpath)) {
			LOG_ENGINE_WARN("Unable to serialize project file");
		}
		if (!m_SceneManager->SerializeScenes(m_ProjectFolderpath)) {
			LOG_ENGINE_WARN("Unable to save scenes");
		}
		if (!m_AssetManager->SerializeAssetPool(m_ProjectFolderpath)) {
			LOG_ENGINE_WARN("Unable to save resources");
		}
		return true;
	}
	
	void ProjectManager::CloseProject() {
		m_ProjectFolderpath = "";
		m_BootSceneGUID = LR_GUID::INVALID;
		m_AssetManager = nullptr;
		m_SceneManager = nullptr;
	}

	bool ProjectManager::SerializeProjFile(const std::filesystem::path& projectFolderpath) const {
		std::filesystem::path filepath = projectFolderpath / (GetProjectFoldername() + PROJECT_FILE_EXTENSION);
		LOG_ENGINE_INFO("Serializing: {0}", filepath);

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "BootSceneGUID" << YAML::Value << (uint64_t)m_BootSceneGUID;
		out << YAML::EndMap;

		// overwrites if exists
		std::ofstream fout(filepath);
		if (!fout.is_open()) {
			LOG_ENGINE_ERROR("Open File Error: {0}", filepath);
			return false;
		}
		fout << out.c_str();
		return true;
	}

	bool ProjectManager::DeserializeProjFile(const std::filesystem::path& projectFolderpath) {
		std::filesystem::path filepath = projectFolderpath / (GetProjectFoldername() + PROJECT_FILE_EXTENSION);
		LOG_ENGINE_INFO("Deserializing: {0}", filepath.string());
		if (!std::filesystem::exists(filepath)) {
			return false;
		}

		YAML::Node root;
		try {
			root = YAML::LoadFile(filepath.string());
		}
		catch (const YAML::ParserException& e) {
			LOG_ENGINE_ERROR("YAML parse error: {}", e.what());
			return false;
		}

		if (root["BootSceneGUID"]) {
			m_BootSceneGUID = (LR_GUID)root["BootSceneGUID"].as<uint64_t>();
		}
		return true;
	}
}