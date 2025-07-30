#include "Project/ProjectManager.h"

namespace Laura
{

	bool ProjectManager::NewProject(std::filesystem::path projectRoot) {
		if (std::filesystem::exists(projectRoot)) {
			return false;
		}
		if (!std::filesystem::create_directory(projectRoot)) {
			return false;
		}
		m_ProjectFolderpath = projectRoot;
		m_BootScene = LR_GUID::INVALID; // no boot scene yet configured
		m_AssetManager = std::make_shared<AssetManager>();
		m_SceneManager = std::make_shared<SceneManager>();
		return SaveProject();
	}

	bool ProjectManager::OpenProject(std::filesystem::path projectRoot) {
		return false;
	}

	bool ProjectManager::SaveProject() {
		return false;
	}
	
	void ProjectManager::CloseProject() {

	}

	bool ProjectManager::SerializeProjFile() const {
		std::filesystem::path filepath = m_ProjectFolderpath / (GetProjectFoldername() + PROJECT_FILE_EXTENSION);
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

	bool ProjectManager::DeserializeProjFile() {
		std::filesystem::path filepath = m_ProjectFolderpath / (GetProjectFoldername() + PROJECT_FILE_EXTENSION);
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